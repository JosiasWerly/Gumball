#pragma once
#ifndef _definitions
#define _definitions

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


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
#endif // !_definitions
