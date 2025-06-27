#pragma once
#include <Rubber/ECS/EcsCommonHeaders.h>


namespace Rubber {

	// tuple_type_index< T > ( tuple) -> return the index of T in tuple if it is there
	template <typename TargetType, typename TupleOfTypes, std::size_t Index>
	struct TupleIndexLookUp;

	// if we hit the target Type
	template<typename TargetType, typename... OtherTypes, std::size_t Index>
	struct TupleIndexLookUp<TargetType, std::tuple<TargetType, OtherTypes...>, Index> :
		std::integral_constant<std::size_t, Index> {
	};

	// if the first type in current Tuple<types..> is not the target Type, recursively eliminate the first nonTargetType
	// until we find the correct type, or we run out of type, which ever comes first
	template<typename TargetType, typename nonTargetType, typename... OtherTypes, std::size_t Index>
	struct TupleIndexLookUp<TargetType, std::tuple<nonTargetType, OtherTypes...>, Index>
		:TupleIndexLookUp<TargetType, std::tuple<OtherTypes...>, Index + 1> {
	};

	template<typename T, typename Tuple, std::size_t Index = 0>
	inline constexpr std::size_t TUpleIndexLookUp_v = TupleIndexLookUp<T, Tuple, Index>::value;
}







