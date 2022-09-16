#pragma once
#ifndef _definitions
#define _definitions

#define Stringfy(s)#s
#define Inline __forceinline


#pragma warning( disable : 4005)
#ifdef GBCORE
#define GBCORE __declspec(dllexport)
#else
#define GBCORE __declspec(dllimport)
#endif

#ifdef GBPROJECT
#define GBPROJECT __declspec(dllexport)
#else
#define GBPROJECT __declspec(dllimport)
#endif

#define Extern extern "C"
#pragma warning( default : 4005)
#endif // !_definitions
