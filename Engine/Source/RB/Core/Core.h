#pragma once


#ifdef RB_DEBUG

#if defined(RB_PLATFORM_WINDOWS)
#define RB_DEBUGBREAK() __debugbreak()
#else
#error "Supported Windows Only!"
#endif 

#ifndef  RB_ENABLE_ASSERTS
#define RB_ENABLE_ASSERTS
#endif


#else 
#define RB_DEBUGBREAK()
#endif 


// define a macro that sets a bit to 1.e.g BIT(1) = 0000 0001
#define BIT(x) (1 << x)





	
