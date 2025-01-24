#pragma once

#include <stdio.h>
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


#define BIT(x) (1 << x)

