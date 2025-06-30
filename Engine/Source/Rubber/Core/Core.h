#pragma once


// define macros that controls the flow  
// of the program
#ifdef WINDOW_64_API
#if DYNAMIC_LINK
	#ifdef RUBBER_BUILD
		#define RB_API  __declspec(dllexport)
	#else
		#define RB_API  __declspec(dllimport)
	#endif
#else
    #define RB_API
#endif
#else
#error Rubber now only supports Window x64
#endif

// define a macro that sets a bit to 1.e.g BIT(1) = 0000 0001
#define BIT(x) (1 << x)

// ref and scope system, currently only use std::shared_ptr for ref and unique_ptr for scoped
// but we can extend it to anything else





	
