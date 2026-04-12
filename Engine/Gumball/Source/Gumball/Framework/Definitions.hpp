#pragma once
#ifndef __definitions
#define __definitions

#define Stringfy(...)#__VA_ARGS__
#define Inline __forceinline

#define Export __declspec(dllexport)
#define Import __declspec(dllimport)

#ifdef GENGINE
#define GENGINE __declspec(dllexport)
#else
#define GENGINE __declspec(dllimport)
#endif

#ifdef GMODULE
#define GMODULE __declspec(dllexport)
#else
#define GMODULE __declspec(dllimport)
#endif

#ifdef GGAME
#define GGAME __declspec(dllexport)
#else
#define GGAME __declspec(dllimport)
#endif

#define Extern extern "C"


using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using s8 = char;
using s16 = short;
using s32 = int;
using s64 = long long;

#ifdef DEBUG
#define DEBUG_ONLY(exp) exp;
#else
#define DEBUG_ONLY(exp)
#endif

#endif // !__definitions
