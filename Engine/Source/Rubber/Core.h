#pragma once

#include  <stdio.h>
#include  <memory>
#include  <utility>


// define macros that controls the flow  
#ifdef WINDOW_64_API
	#ifdef RUBBER_BUILD
		#define RB_API  __declspec(dllexport)
	#else
		#define RB_API  __declspec(dllimport)
	#endif
#else
#error Rubber now only supports Window x64
#endif

// define a macro that sets a bit to 1.e.g BIT(1) = 0000 0001
#define BIT(x) (1 << x)


template <typename T>
constexpr std::underlying_type_t<T> toUnderType(T enumarator)
{
	return static_cast<std::underlying_type_t<T>> (enumarator);
}
