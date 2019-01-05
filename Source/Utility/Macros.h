#pragma once
#include <Logger/Log.h>

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) {if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)