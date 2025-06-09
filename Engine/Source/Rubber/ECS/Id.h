#pragma once
#include <cstdint>
#include "Rubber/Core/Primitives.h"
#include "Rubber/Core/Assert.h"
#include "Rubber/log/Log.h"
#include <typeinfo>


namespace Rubber::ID{

	using IdType = uint32_t;

	// generation bits (we use 8 bits for now to record the generation info for the ID
	inline constexpr uint32_t	generationBits{ 8};

	//the rest of the bits are used to record index info
	inline constexpr uint32_t	indexBits{ sizeof(uint32_t) * 8 - generationBits };

	// creating a index mask so we can strip out the index info example usage index = id & indexBits
	inline constexpr IdType  indexMask{ (IdType{1} << indexBits) - 1 };

    // creating a generation mask so we can strip out the generation info
	inline constexpr IdType  generationMask{ (IdType{1} << generationBits) - 1 };

	// invalid ID 
	inline constexpr IdType invalidId{ IdType(-1) };

	// we have to delete at least 1024 elements before we start to overwrite used slot (generation + 1)
	inline constexpr uint32_t  minDeletedElements{ 1024 };

	// find the most suitable type to represents the generation number
	using GenerationType = ::std::conditional_t<generationBits <= 16, ::std::conditional_t<generationBits <= 8, uint8_t, uint16_t>, uint32_t>;
	RB_STATIC_ASSERT(sizeof(GenerationType) * 8 >= generationBits, "Make sure the generation Type is large enough for its size!");
	RB_STATIC_ASSERT(sizeof(IdType) - sizeof(GenerationType) > 0, "Make sure the generation type is not large than the IdType!");

	inline constexpr IdType invalidGeneration{ (IdType{1} << generationBits) - (IdType)1 }; // 255 for 8 bits, for example
	inline constexpr IdType inValidIndex{ (IdType{1} << indexBits) - IdType(1) };

	// check if the id is valid
	constexpr bool isValidID(IdType id) {
		return id != invalidId;
	}

	constexpr bool isValidIndex(IdType index){
		return  index < inValidIndex;
	}

	constexpr bool isValidGeneration(IdType generaiton){
		return  generaiton < invalidGeneration;
	}

	// return the index value of a given id
	constexpr IdType indexOf(IdType id){
		IdType index{ id & indexMask };
		RB_CORE_ASSERT(index != invalidId, "Invalid Index Value");
		return index;
	}

	// return the generation value of a given id, example : id = 1001 -> first shift right by indexbits(3) 
	// ->  0001; generation mask = 0001 -> left shift by generationBits -> 0010 -> -1 -> 0001, 0001 & 0001 = 0001;
	constexpr IdType generationOf(IdType id){
		return  (id >> indexBits) & generationMask ;
	}

	// update the generation value without touching on the index  1001->2001, return the id after the update
	inline IdType newGeneration(IdType id){
		const IdType newGenVal =  generationOf(id) + 1;
		RB_CORE_ASSERT(isValidGeneration(newGenVal), "NewGenNuber is not valid!");
		return isValidGeneration(newGenVal) ? ((newGenVal << indexBits) | indexOf(id)) : invalidId;
	}

// strong type for debug purpose, do not allow implicit  
#ifdef _DEBUG
	namespace Internal {
		struct IdBase {
			constexpr explicit IdBase(IdType id) : m_ID(id) {}
			constexpr operator IdType() const { return this->m_ID; }
		private:
			IdType m_ID;
		};
	}

#define DEFINE_TYPED_ID(name)                                  \
	struct name: ID::Internal::IdBase {                        \
			constexpr explicit name(ID::IdType id)             \
			: ID::Internal::IdBase(id) {}                                    \
			constexpr name() : ID::Internal::IdBase(0){}                     \
	};

#else
#define DEFINE_TYPED_ID(name)  using name = Id::IdType

#endif

} // namespace ECS::ID

