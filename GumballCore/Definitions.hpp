#pragma once
#ifndef _definitions
#define _definitions

#define Stringfy(s)#s
#define Inline __forceinline

#ifdef DLLExport
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#define Extern extern "C" DLL


#endif // !_definitions
