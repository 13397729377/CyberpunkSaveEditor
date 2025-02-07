#pragma once
#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
  #define NOMINMAX
#endif

#if defined(_MSC_VER )
#define DEBUG_BREAK __debugbreak
#define FUNCSIG __FUNCSIG__
#define FORCE_INLINE __forceinline
#else
#error compiler not supported
#endif

#include <inttypes.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#define CP_ENSURE(expr) do { if (!(expr)){ SPDLOG_CRITICAL("assertion failed: ({})", #expr); DEBUG_BREAK(); } } while(0)

#ifdef _DEBUG
#define CP_ASSERT(expr) CP_ENSURE(expr)
#else
#define CP_ASSERT(expr)
#endif

#ifdef _DEBUG
#define CP_VERIFY(expr) CP_ENSURE(expr)
#else
#define CP_VERIFY(expr) std::ignore = (expr)
#endif
