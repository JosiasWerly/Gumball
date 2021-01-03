#pragma once
#ifndef _script
#define _script

#define expDLL

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

class DLL ChildScript : 
	public BaseScript {
public:
	void test();
};

Extern double add(double a, double b);
Extern BaseScript* createBaseScript();
#endif // !_script
