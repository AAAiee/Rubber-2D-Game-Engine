#pragma once
#include "Core.h"
#include <filesystem>

#ifdef RB_ENABLE_ASSERTS
	// ASSERT IMPLEMENTATION
	#define RB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
			do { if (!(check)) { type##_ERROR(msg, ##__VA_ARGS__); RB_DEBUGBREAK(); } } while (false)

	#define RB_INTERNAL_ASSERT_WITH_MESSAGE(type, check, ...) \
			RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion Failed: {0}", __VA_ARGS__)

	#define RB_INTERNAL_ASSERT_NO_MESSAGE(type, check, ...)  \
			RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1} : {2}", #check, std::filesystem::path(__FILE__).filename().string(), __LINE__)

	//RB_GET_MACRO chooses the correct assert macro to delegate depends on number of parameters passed in
	// e.g. RB_CORE_ASSERT(false) -> RB_GET_MACRO(false, RB_CORE_ASSERT_WITH_MESSAGE, RB_CORE_ASSERT_NO_MESSAGE) -> RB_CORE_ASSERT_NO_MESSAGE(false)
	#define RB_INTERNAL_GET_MACRO_NAME(args1, args2, correctMacroName, ...) correctMacroName
	#define RB_INTERNAL_ASSERT_GET_MACRO(...) RB_INTERNAL_GET_MACRO_NAME(__VA_ARGS__, RB_INTERNAL_ASSERT_WITH_MESSAGE, RB_INTERNAL_ASSERT_NO_MESSAGE)

	#define RB_CORE_ASSERT(...) RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(RB,__VA_ARGS__)
	#define RB_ASSERT(...)  RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(APP,__VA_ARGS__)

	//Static Assert
	#define RB_STATIC_ASSERT(cond,msg)  static_assert((cond), msg);

#else
	#define RB_ASSERT(x,...)  
	#define RB_CORE_ASSERT(x, ...)  
	#define RB_STATIC_ASSERT(cond, msg)
#endif

