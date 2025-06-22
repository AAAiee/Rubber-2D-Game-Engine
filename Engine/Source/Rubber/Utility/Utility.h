#pragma once

#define USE_STL_VECTOR 1
#define USE_STL_DEQUE 1
#define USE_STL_SMART_POINTER 1


#if USE_STL_VECTOR
#include <vector>
namespace Rubber{
	template<typename T>
	using Vector = std::vector<T>;
}
#endif

#if USE_STL_VECTOR
#include <deque>
namespace Rubber{
	template<typename T>
	using Deque = std::deque<T>;
}
#endif

#if  USE_STL_SMART_POINTER
#include <memory>
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> makeRef(Args&&... args)
{
	// If one day you swap in a custom memory pool,
	// you only touch this line.
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr Scope<T> makeScope(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif

// if we want to implement our own version of vector or deque

namespace Rubber{

}
