// unnecessary to include this header directly.
// Designed to support C++17 or later, but only tested under C++20.
#pragma once

#ifndef CONSTEXPR
#if _HAS_CXX20
#define CONSTEXPR constexpr
#else
#define CONSTEXPR
#endif
#endif
