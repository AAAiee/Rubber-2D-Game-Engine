#include "Rubber/ECS/EcsCommonHeaders.h"
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <new>
#include <random>
#include <cstring>
#include <cassert>


namespace Rubber {

	namespace {

		std::random_device rd;
		std::mt19937 randomEngine{ rd() };
		std::uniform_int_distribution<uint32_t>  dist{ 0, Rubber::ID::inValidIndex - 1 };

		inline constexpr uint8_t RESCALE_FACTOR = static_cast<uint8_t>(2);
	}

	namespace {
		// takes in a schema and output a tuple with corresponding pointer types
		template<class Tup> struct PtrTuple;
		template<class... Ts> struct PtrTuple<std::tuple<Ts...>>
		{
			using type = std::tuple<Ts*...>;
		};
		template<class Tup> using PtrTuple_t = typename PtrTuple<Tup>::type;
 
		// find the aligned offset for each component, for example, a data structure with
		// alignment of 4 will start at an offset that is a multiple of 4
		inline std::size_t roundUp(std::size_t off, std::size_t a)
		{
			std::size_t r = off % a;
			return r ? off + (a - r) : off;
		}
	}


	// the base class for all components, in charge of basic functionalities such as
	// allocate AOS to store component's data, garbage collector, general lookup.., all 
	// other component should be generated from this class template. for now only works for POD types!
	template<class SCHEMA>
	class ComponentManager
	{
		using Types = typename SCHEMA::Types;     // tuple <T1,T2,…>
		using ColPtrs = PtrTuple_t <Types>;        // tuple <T1*,T2*,…>
		using ColIndex = typename SCHEMA::Order;

		// define the AOS structure
		struct Block {
			// n: current active instances 
			// cap: capacity of the AOS
			std::size_t n{}, cap{};
 
			// buf: the AOS
			void* buf{};
 
			// entities pointer points to the start position of entity data in  the AOS
			Entity* ent{};

			// the rest of the components, passed by the SCHEMA, their pointers in the AOS
			ColPtrs col{};
		};

	public:
		// constructor that default has a capacity for 16 instances
		ComponentManager() { grow(16); }

		// destructor that free up the AOS
		~ComponentManager() { ::operator delete(m_Data.buf, std::align_val_t(m_MaxAlign)); }

		// general look up method that returns an instance(an wrapper for an index)
		// this is to help the client who wants access to the same component without repeated
		// look up
		Component::Instance lookup(Entity entity) const {
			if (this->m_Map[ID::indexOf(entity.id)] == ID::invalidId) {
				return Component::makeInstance(ID::invalidId);
			}
			return Component::makeInstance(m_Map[ID::indexOf(entity.id)]);
		}


		// garbage Collector
		void garbageCollector(const Rubber::EntityManager& em)
		{
			int32_t aliveInRow = 0;
			uint32_t n;
			while ((n = m_Data.n) > 0 && aliveInRow < 4) {
				uint32_t randomIndex = dist(randomEngine) % n;
				if (em.is_alive(this->m_Data.ent[randomIndex].id)) {
					aliveInRow++;
					continue;
				}

				aliveInRow = 0;
				removeComponent(randomIndex);
			}
		}

		void removeComponent(uint32_t index){
			uint32_t last = this->m_Data.n - 1;
			RB_CORE_ASSERT(index < this->m_Data.n, "index does not exists!");

			Entity lastInArray = this->m_Data.ent[last];
			Entity toDelete = this->m_Data.ent[index];

			if(index < last){
				this->m_Data.ent[index] = lastInArray;

				// copy the data from the last entities to index's pos
				copyElement(m_Data.col, index, last, std::make_index_sequence<std::tuple_size_v<ColPtrs>>());

				this->m_Map[ID::indexOf(lastInArray.id)] = index;
			}

			this->m_Map[ID::indexOf(toDelete.id)] = ID::invalidId;
			--this->m_Data.n;
		}

		void addComponent(Types data, Entity owner) {
			if (this->m_Data.n >= this->m_Data.cap) {
				size_t resizeTo =  this->m_Data.cap * RESCALE_FACTOR;
				grow(resizeTo);
			}
			// make sure the entity's id now gives correct index to the correct transform instance
			std::size_t insertPos = m_Data.n;

			this->m_Map[ID::indexOf(owner.id)] = static_cast <uint32_t> (insertPos);

			// add data over;
			this->m_Data.ent[insertPos] = owner;

			// now suppose every data passed in is correct
			insertElement(std::make_index_sequence<std::tuple_size_v<Types>> {}, data, insertPos);
			this->m_Data.n++;
		}

		size_t count () const{
			return  m_Data.n;

		}


		template<ColIndex C>
		using ColType = std::tuple_element_t<static_cast<std::size_t>(C), Types>;


		template<ColIndex C>
	    ColType<C>& get(Entity entity)  {
			auto instance = lookup(entity);
			RB_CORE_ASSERT(instance.index != ID::invalidId, "entity has no such component");
			return get<C>(instance);
		}

		template<ColIndex C>
		 ColType<C>& get(Component::Instance instance)  {
			return  std::get<static_cast<std::size_t>(C)>(m_Data.col)[instance.index];
		}

		template<ColIndex C>
		void set(Entity entity, const ColType<C>& data) {
			auto instance = lookup(entity);
			RB_CORE_ASSERT(instance.index != ID::invalidId, "entity has no such component");
			set<C>(instance,data);
		}

		template<ColIndex C>
		void set(Component::Instance instance, const ColType<C>& data) {
			 std::get<static_cast<std::size_t>(C)>(m_Data.col)[instance.index] = data;
		}


	private:
		Block       m_Data{};
		std::size_t m_MaxAlign{ 1 };
		Utility::vector<uint32_t> m_Map{};

		// ── grow / allocate ───────────────────────────────────────────────────
		void grow(std::size_t newCap)
		{
			assert(newCap > m_Data.n);

			// 1) maximum alignment (include Entity)
			Types d{}; // dummy where we use to extract the alignment of each type
			m_MaxAlign = std::apply([&](auto... x) {

				return std::max({ std::size_t(alignof(Entity)),
								  std::size_t(alignof(std::decay_t<decltype(x)>))... });
				},d);

			// 2) total bytes
			std::size_t off = 0;
			off = roundUp(off, alignof(Entity));
			off += newCap * sizeof(Entity);
			ColPtrs dummyPtrs{}; // dummy where we use to extract the total bytes

			std::apply([&](auto*... ptr) {
				((off = roundUp(off, alignof(std::remove_pointer_t<decltype(ptr)>)),
					off += newCap * sizeof(std::remove_pointer_t<decltype(ptr)>)), ...);
				}, dummyPtrs);

			std::size_t total = off;

			// 3) allocate one slab
			Block nxt;
			nxt.n = m_Data.n;
			nxt.cap = newCap;
			nxt.buf = ::operator new(total, std::align_val_t(m_MaxAlign));

			char* base = static_cast<char*>(nxt.buf);
			off = 0;

			// carve Entities array
			off = roundUp(off, alignof(Entity));
			nxt.ent = reinterpret_cast<Entity*>(base + off);
			off += newCap * sizeof(Entity);

			// carve every column
			std::apply([&](auto*&... p) {
				((off = roundUp(off, alignof(std::remove_pointer_t<decltype(p)>)),
					p = reinterpret_cast<std::remove_pointer_t<std::remove_reference_t<decltype(p)>>*>(base + off),
					off += newCap * sizeof(std::remove_pointer_t<decltype(p)>)), ...);
				}, nxt.col);

			// 4) copy old data
			std::memcpy(nxt.ent, m_Data.ent, sizeof(Entity) * nxt.n);

			auto old = m_Data.col;

			constexpr std::size_t N = std::tuple_size_v<decltype(old)>;
			copyAll(nxt.col, old, nxt.n, std::make_index_sequence<N>{});

			// 5) replace
			::operator delete(m_Data.buf, std::align_val_t(m_MaxAlign));
			m_Data = nxt;
			this->m_Map.resize(nxt.cap, static_cast<EntityID>(ID::invalidId));
		}


		// ── tuple-copy helpers ────────────────────────────────────────────────
		template<std::size_t I, class NewT, class OldT>
		static void copyCol(NewT& dst, OldT& src, std::size_t n)
		{
			using T = std::tuple_element_t<I, OldT>;
			std::memcpy(std::get<I>(dst), std::get<I>(src), sizeof(T) * n);
		}

		template<std::size_t... Is, class NewT, class OldT>
		static void copyAll(NewT& dst, OldT& src, std::size_t n,
			std::index_sequence<Is...>)
		{
			(copyCol<Is>(dst, src, n), ...);
		}

		template<std::size_t... Is>
		static void copyElement(ColPtrs& cols, std::size_t dst, std::size_t src, std::index_sequence<Is...>){
			((std::get<Is>(cols)[dst] = std::get<Is>(cols)[src]), ...);
		}

		template<std::size_t... Is>
		void insertElement(std::index_sequence<Is...>,
			const Types& data,
			std::size_t dst)
		{
			((std::get<Is>(m_Data.col)[dst] =
				std::get<Is>(data)), ...);
		}


	};
}
