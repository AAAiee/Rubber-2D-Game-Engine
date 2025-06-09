#pragma once

// define assert macros, when !x, log and debug break
#ifdef RB_ENABLE_ASSERT  
#define RB_ASSERT(x,...)   do{if(!(x)) {APP_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}} while (false)  
#define RB_CORE_ASSERT(x,...)   do{if (!(x)) {RB_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}} while (false)

#define RB_STATIC_ASSERT(cond,msg)  static_assert((cond), msg);
#else
#define RB_ASSERT(x,...)  
#define RB_CORE_ASSERT(x, ...)  
#define RB_STATIC_ASSERT(cond, msg)
#endif