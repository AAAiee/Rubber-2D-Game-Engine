#pragma once
#include  <pch.h>


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


// define assert macros, when !x, log and debugbreak
#ifdef RB_ENABLE_ASSERT  
   #define RB_ASSERT(x,...)  do{ if(!(x)) {ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}} while (0)  
   #define RB_CORE_ASSERT(x,...)  do{if (!(x)) {RB_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}} while (0)
#else
    #define RB_ASSERT(x,...)  
    #define RB_CORE_ASSERT(x, ...)  
#endif



template <typename T>
constexpr std::underlying_type_t<T> toUnderType(T enumarator)
{
	return static_cast<std::underlying_type_t<T>> (enumarator);
}
