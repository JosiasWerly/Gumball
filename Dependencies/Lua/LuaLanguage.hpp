#pragma once
#ifndef _lualanguage
#define _lualanguage

#include "lua/lua.hpp"

#include <string>
using namespace std;
class LuaVirtualMachine {
private:
	lua_State *L;
public:
	LuaVirtualMachine() {
		L = luaL_newstate();
		luaL_openlibs(L);
		
	}
	~LuaVirtualMachine() {
		lua_close(L);
	}
	lua_State *luaState() {
		return L;
	}
	string log() {
		return lua_tostring(L, -1);
	}
	
	bool evalScript(string path) {
		return luaL_dofile(L, path.c_str()) == LUA_OK; //lua_load
	}
	bool eval(string command) {
		return luaL_dostring(L, command.c_str()) == LUA_OK;
	}


	template<class T> void setValue(string, T) = delete;
	template<> void setValue(string name, bool value) {
		lua_pushboolean(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, int value) {
		lua_pushinteger(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, double value) {
		lua_pushnumber(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, string value) {
		lua_pushstring(L, value.c_str());
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};

	template<class T> T getValue(string name) = delete;
	template<> bool getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isboolean(L, -1)) {
			return lua_toboolean(L, -1);
		}
		return false;
	}
	template<> int getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isnumber(L, -1)) {
			return (int)lua_tointeger(L, -1);;
		}
		return 0;
	}
	template<> double getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isnumber(L, -1)) {
			return (double)lua_tonumber(L, -1);
		}
		return 0;
	}
	template<> string getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isstring(L, -1)) {
			size_t sz = 0;
			return lua_tolstring(L, -1, &sz);
		}
		return "";
	}



	bool runLuaFunction(string fnxName) {
		lua_getglobal(L, fnxName.c_str());
		if (lua_isfunction(L, -1)) {
			lua_pcall(L, 0, 0, 0);
			return true;
		}
		return false;
	}
	void del(string name) {
		lua_getglobal(L, name.c_str());
		lua_pop(L, -1);
	}

	void pushFunction(string fnxName, lua_CFunction fnx) {
		lua_register(L, fnxName.c_str(), fnx);
	}
	void popFunction(string fnxName) {
		lua_getglobal(L, fnxName.c_str());
		if (lua_isfunction(L, -1))
			lua_remove(L, -1);
	}
};


//class LuaVirtualMachine {
//private:
//	lua_State *L;
//public:
//	LuaVirtualMachine() {
//		L = luaL_newstate();
//		luaL_openlibs(L);
//	}
//	~LuaVirtualMachine() {
//		lua_close(L);
//	}
//	lua_State *luaState() {
//		return L;
//	}
//	string log() {
//		return lua_tostring(L, -1);
//	}
//	bool evalScript(string path) {
//		return luaL_dofile(L, path.c_str()) == LUA_OK; //lua_load
//	}
//	bool eval(string command) {
//		return luaL_dostring(L, command.c_str()) == LUA_OK;
//	}
//	bool runFunction(string fnxName) {
//		lua_getglobal(L, fnxName.c_str());
//		if (lua_isfunction(L, -1)) {
//			lua_pcall(L, 0, 0, 0);
//			return true;
//		}
//		return false;
//	}
//
//	template<class T> void setValue(string, T) {};
//	template<> void setValue(string name, double value) {
//		lua_pushnumber(L, value);
//		lua_setglobal(L, name.c_str());
//		lua_pop(L, -1);
//	};
//	template<> void setValue(string name, int value) {
//		lua_pushinteger(L, value);
//		lua_setglobal(L, name.c_str());
//		lua_pop(L, -1);
//	};
//	template<> void setValue(string name, bool value) {
//		lua_pushboolean(L, value);
//		lua_setglobal(L, name.c_str());
//		lua_pop(L, -1);
//	};
//	template<> void setValue(string name, string value) {
//		lua_pushstring(L, value.c_str());
//		lua_setglobal(L, name.c_str());
//		lua_pop(L, -1);
//	};
//
//	template<class T> T getValue(string name) { return 0; }
//	template<> int getValue(string name) {
//		lua_getglobal(L, name.c_str());
//		if (lua_isnumber(L, -1)) {
//			return (int)lua_tointeger(L, -1);;
//		}
//		return 0;
//	}
//	template<> double getValue(string name) {
//		lua_getglobal(L, name.c_str());
//		if (lua_isnumber(L, -1)) {
//			return (double)lua_tonumber(L, -1);
//		}
//		return 0;
//	}
//	template<> bool getValue(string name) {
//		lua_getglobal(L, name.c_str());
//		if (lua_isboolean(L, -1)) {
//			return lua_toboolean(L, -1);
//		}
//		return false;
//	}
//	template<> string getValue(string name) {
//		lua_getglobal(L, name.c_str());
//		if (lua_isstring(L, -1)) {
//			size_t sz = 0;
//			return lua_tolstring(L, -1, &sz);
//		}
//		return "";
//	}
//	void del(string name) {
//		lua_getglobal(L, name.c_str());
//		lua_pop(L, -1);
//	}
//
//	void pushFunction(string fnxName, lua_CFunction fnx) {
//		lua_register(L, fnxName.c_str(), fnx);
//	}
//	void popFunction(string fnxName) {
//		lua_getglobal(L, fnxName.c_str());
//		if (lua_isfunction(L, -1))
//			lua_remove(L, -1);
//	}
//};
//
//#include<iostream>
//static int Test(lua_State *L) {
//	int arguments = lua_gettop(L);
//	//string args;
//
//	float sum = 0;
//	for (int x = 1; x <= arguments; x++)
//		sum += (float)lua_tonumber(L, x);
//	//std::cout << args << std::endl;
//	lua_pushstring(L, "ret1");
//	lua_pushnumber(L, sum);
//	return 2;
//}
//class LuaScript {
//	LuaVirtualMachine *luaTarget = new LuaVirtualMachine;
//	string absfilePath;
//public:
//	LuaScript() {
//		luaTarget->pushFunction("Test", Test);
//	}
//	void loadScript(string filePath) {
//		luaTarget->evalScript(filePath);
//	}
//	void beginPlay() {
//		luaTarget->runFunction("setup");
//	}
//	void tick() {
//		luaTarget->runFunction("tick");
//	}
//};


#endif // !_lualanguage
