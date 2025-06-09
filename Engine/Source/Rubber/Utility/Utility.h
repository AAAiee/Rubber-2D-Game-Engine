#pragma once

#define USE_STL_VECTOR 1
#define USE_STL_DEQUE 1


#if USE_STL_VECTOR
#include <vector>
namespace Rubber::Utility{
	template<typename T>
	using vector = std::vector<T>;
}

#endif

#if USE_STL_VECTOR
#include <deque>
namespace Rubber::Utility{
	template<typename T>
	using deque = std::deque<T>;
}
#endif

// if we want to implement our own version of vector or deque

namespace Rubber::Utility{

	template<std::size_t Align>
	struct AlignedAllocator {

		static void* allocate(size_t size) {

			void* memory = ::operator new(size, Align);
			if (!memory){
				throw std::bad_alloc();
			}
		}

		static void deallocate(void* location){
			::operator delete(location);
		}
	};
}
