#pragma once

/**
	Pre-compiled headers allow for faster compile times.
	Place headers in here that are relatively stable.
**/

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

// Custom Library
#include <glm.hpp>

// User-Made Headers
#include <Logger/Log.h>
#include <Macros.h>


#ifdef FED_PLATFORM_WINDOWS
#include <Windows.h>
#endif

// __debugbreak is an MSVC intrinsic; provide a portable equivalent for
// clang/emcc so the existing call sites (Macros.h ASSERT, VertexBuffer.cpp)
// compile on the wasm build. __builtin_trap aborts the program with a SIGTRAP-
// equivalent on wasm, which surfaces in the browser DevTools console.
#if !defined(_MSC_VER)
#  if defined(__clang__) || defined(__GNUC__)
#    define __debugbreak() __builtin_trap()
#  else
#    include <cstdlib>
#    define __debugbreak() std::abort()
#  endif
#endif