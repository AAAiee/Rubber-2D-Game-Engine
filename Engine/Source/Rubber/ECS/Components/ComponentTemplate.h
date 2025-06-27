#pragma once
#include "Rubber/ECS/EcsCommonHeaders.h"
#include "Rubber/Utility/Utility.h"

// add concept to limit the type?
namespace Rubber {

	namespace {

		inline constexpr uint8_t RESCALE_FACTOR = static_cast<uint8_t>(2);

		// takes in a schema and output a tuple with corresponding pointer types
		template<class TupleOfTypes> struct TupleOfTypePtrs;

		template<class... Ts> struct TupleOfTypePtrs<std::tuple<Ts...>>
		{
			using type = std::tuple<Ts*...>;
		};

		template<class TupleOfTypes> using TupleOfTypePtrs_t = typename TupleOfTypePtrs<TupleOfTypes>::type;
 
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
	template<typename... ColTypes>
	class ComponentBase
	{
		using TupleColTypes = std::tuple<ColTypes...>;
		using ColPtrs = TupleOfTypePtrs_t <TupleColTypes>;        // tuple <T1*,T2*,…>

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
		ComponentBase() { grow(16); }

		// destructor that free up the AOS
		~ComponentBase() { ::operator delete(m_Data.buf, std::align_val_t(m_MaxAlign)); }

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
			uint32_t aliveInRow = 0;
			while ( m_Data.n > 0 && aliveInRow < 4ui32) {
				uint32_t randomIndex = (uint32_t)(RandomEngine::Float() * (float)m_Data.n);
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

		template <typename... Args>
		void addComponent(Entity owner, Args&&... values) {
			RB_STATIC_ASSERT(sizeof...(Args) == sizeof...(ColTypes), "Entity lacks this specific component!");

			if (this->m_Data.n >= this->m_Data.cap) {
				size_t resizeTo =  this->m_Data.cap * RESCALE_FACTOR;
				grow(resizeTo);
			}
			// make sure the entity's id now gives correct index to the correct transform instance
			std::size_t insertPos = m_Data.n;

			this->m_Map[ID::indexOf(owner.id)] = static_cast <uint32_t> (insertPos);

			// add data to the right place;
			this->m_Data.ent[insertPos] = owner;

			// now suppose every data passed in is correct
			insertElement(std::index_sequence_for<ColTypes...> {}, insertPos, std::forward<Args>(values)...) ;
			this->m_Data.n++;
		}

		size_t count() const{
			return  m_Data.n;
		}


		template<typename TargetType>
		TargetType& get(Entity entity){
			auto instance = lookup(entity);
			RB_CORE_ASSERT(instance.index != ID::invalidId, "entity has no such component");
			return  get<TargetType>(instance);
		}

		template<typename TargetType>
		TargetType& get(Component::Instance instance){
			return  std::get<TUpleIndexLookUp_v<TargetType, TupleColTypes>>(m_Data.col)[instance.index];
		}

		template< typename TargetType>
		void set(Entity entity, const TargetType& data) {
			auto instance = lookup(entity);
			RB_CORE_ASSERT(instance.index != ID::invalidId, "entity has no such component");
			set<TargetType>(instance,data);
		}

		template <typename TargetType>
		void set(Component::Instance instance, const TargetType& data){
			TargetType& curVal = get<TargetType>(instance);
			curVal = data;
		} 

	private:
		Block       m_Data{};
		std::size_t m_MaxAlign{ 1 };
		Vector<uint32_t> m_Map;

		// ── grow / allocate ───────────────────────────────────────────────────
		void grow(std::size_t newCap)
		{
			RB_CORE_ASSERT(newCap > m_Data.n, "new Capacity should be able to store all current existed data!");

			// 1) maximum alignment (include Entity)
			TupleColTypes d{}; // dummy where we use to extract the alignment of each type
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

		template<std::size_t... Is, typename... Args>
		void insertElement(std::index_sequence<Is...>, std::size_t dst, Args&&... data)
		{
			((std::get<Is>(m_Data.col)[dst] = std::forward<Args>(data)), ...);
		}
	};
}
