#pragma once
#ifndef _script
#define _script

#ifdef expDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
#define Extern extern "C" DLL

class DLL BaseScript{
public:
	virtual void test();
};
//Extern OnProjectPlay
#endif // !_script
