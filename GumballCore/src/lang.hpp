#pragma once
#ifndef _lang
#define _lang
/*

//http://gamedevgeek.com/tutorials/calling-c-functions-from-lua/
//#ifndef LUA_OK
//#define LUA_OK 0;
//#endif // !LUA_OK
#include <string>

#include <lua/lua.hpp>
using namespace std;
class LuaVirtualMachine {
private:
	lua_State* L;
public:
	LuaVirtualMachine() {
		L = luaL_newstate();
		luaL_openlibs(L);
		//lua_register(L, "Get", Get);
		//lua_register(L, "Set", Set);
		//lua_register(L, "New", New);
		//lua_register(L, "Del", Del);
		//lua_register(L, "Save", Save);
	}
	~LuaVirtualMachine() {
		lua_close(L);
	}
	lua_State* luaState() {
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
	bool runFunction(string fnxName) {
		lua_getglobal(L, fnxName.c_str());
		if (lua_isfunction(L, -1)) {
			lua_pcall(L, 0, 0, 0);
			return true;
		}
		return false;
	}

	template<class T> void setValue(string, T) {};
	template<> void setValue(string name, double value) {
		lua_pushnumber(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, int value) {
		lua_pushinteger(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, bool value) {
		lua_pushboolean(L, value);
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};
	template<> void setValue(string name, string value) {
		lua_pushstring(L, value.c_str());
		lua_setglobal(L, name.c_str());
		lua_pop(L, -1);
	};

	template<class T> T getValue(string name) { return 0; }
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
	template<> bool getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isboolean(L, -1)) {
			return lua_toboolean(L, -1);
		}
		return false;
	}
	template<> string getValue(string name) {
		lua_getglobal(L, name.c_str());
		if (lua_isstring(L, -1)) {
			size_t sz = 0;
			return lua_tolstring(L, -1, &sz);
		}
		return "";
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

#include<iostream>
static int Test(lua_State* L) {
	int arguments = lua_gettop(L);
	//string args;

	float sum = 0;
	for(int x=1; x<=arguments; x++)
		sum += (float)lua_tonumber(L, x);
	//std::cout << args << std::endl;
	lua_pushstring(L, "ret1");
	lua_pushnumber(L, sum);
	return 2;
}
class LuaScript{
	LuaVirtualMachine* luaTarget = new LuaVirtualMachine;
	string absfilePath;
public:
	LuaScript() {
		luaTarget->pushFunction("Test", Test);
	}
	void loadScript(string filePath) {
		luaTarget->evalScript(filePath);
	}
	void beginPlay() {
		luaTarget->runFunction("setup");
	}
	void tick() {
		luaTarget->runFunction("tick");
	}
};
*/













//LuaScript gameplay;
//gameplay.loadScript("C:\\Users\\ADM\\Downloads\\fun.lua");
//gameplay.beginPlay();
//gameplay.tick();


//	class LuaScripts :
//		public LuaVirtualMachine {
//	public:
//		QString absfilePath, fileName;
//		LuaScriptState state;
//
//		VariableList binders;
//		void updateEnvVars() {
//			auto& ls = binders.getList();
//			for each (auto & var in ls) {
//				Types::eTypes type = (*var)().getType();
//				switch (type) {
//				case Types::eTypes::Bool:
//					*var = getValue<bool>(var->name);
//					break;
//				case Types::eTypes::Char:
//					*var = (char)getValue<int>(var->name);
//					break;
//				case Types::eTypes::Short:
//					*var = (short)getValue<int>(var->name);
//					break;
//				case Types::eTypes::Int:
//					*var = getValue<int>(var->name);
//					break;
//				case Types::eTypes::Float:
//					*var = (float)getValue<double>(var->name);
//					break;
//				case Types::eTypes::Long:
//					*var = (long)getValue<int>(var->name);
//					break;
//				case Types::eTypes::Double:
//					*var = (double)getValue<double>(var->name);
//					break;
//				case Types::eTypes::String:
//					*var = getValue<QString>(var->name);
//					break;
//				}
//			}
//		}
//		void updateLuaVars() {
//			auto& ls = binders.getList();
//			for each (auto & var in ls) {
//				Types::eTypes type = (*var)().getType();
//				switch (type) {
//				case Types::eTypes::Bool:
//					setValue<bool>(var->name, (bool)(*var));
//					break;
//				case Types::eTypes::Char:
//					setValue<int>(var->name, (char)(*var));
//					break;
//				case Types::eTypes::Short:
//					setValue<int>(var->name, (short)(*var));
//					break;
//				case Types::eTypes::Int:
//					setValue<int>(var->name, (int)(*var));
//					break;
//				case Types::eTypes::Float:
//					setValue<double>(var->name, (float)(*var));
//					break;
//				case Types::eTypes::Long:
//					setValue<int>(var->name, (long)(*var));
//					break;
//				case Types::eTypes::Double:
//					setValue<double>(var->name, (double)(*var));
//					break;
//				case Types::eTypes::String:
//					setValue<QString>(var->name, (*var));
//					break;
//				}
//			}
//		}
//	};
//	class LuaEnviroment :
//		public Singleton<LuaEnviroment> {
//		GlobalVariables& gvar = GlobalVariables::instance();
//
//		QList<LuaScripts*> availableScripts;
//		QList<LuaScripts*> toSetup;
//		QList<LuaScripts*> toRun;
//
//		LuaScripts* findInList(QList<LuaScripts*>& ls, QString name) {
//			for each (auto sc in ls) {
//				if (sc->fileName == name) {
//					return sc;
//				}
//			}
//			return nullptr;
//		}
//		LuaScripts* findInListLuaState(QList<LuaScripts*>& ls, lua_State* lstate) {
//			for each (auto sc in ls) {
//				if (sc->luaState() == lstate) {
//					return sc;
//				}
//			}
//			return nullptr;
//		}
//		bool removeInList(QList<LuaScripts*>& ls, QString name) {
//			for (int i = 0; i < ls.size(); i++) {
//				if (ls[i]->fileName == name) {
//					ls.removeAt(i);
//					return true;
//				}
//			}
//			return false;
//		}
//	public:
//		QList<LuaScripts*>& scripts() {
//			return availableScripts;
//		}
//		void init() {
//			auto ls = FileManangement::filesInDirectory(GlobalVariables::instance().filterPath, "lua");
//			for each (auto f in ls) {
//				QString AbsfilePath = f.absoluteFilePath();
//				AbsfilePath = AbsfilePath.replace("/", "\\");
//				LuaScripts* luaScript = new LuaScripts;
//				availableScripts << luaScript;
//				luaScript->fileName = f.baseName();
//				luaScript->state = LuaScriptState::unloaded;
//				luaScript->absfilePath = AbsfilePath;
//			}
//			auto arr = gvar.configs["LuaEnviroment"].toObject()["Scripts"].toArray();
//			for (auto& v : arr) {
//				this->load(v.toString());
//			}
//		}
//		void shutdown() {
//			QJsonObject jScripts;
//			QJsonArray arr;
//			for each (auto enable in toRun) {
//				arr.append(enable->fileName);
//			}
//			jScripts["Scripts"] = arr;
//			gvar.configs["LuaEnviroment"] = jScripts;
//
//
//			for (size_t i = 0; i < availableScripts.size(); i++) {
//				delete availableScripts[i];
//				availableScripts[i] = nullptr;
//			}
//			availableScripts.clear();
//			toSetup.clear();
//			toRun.clear();
//		}
//
//		bool unload(QString scriptName) {
//			LuaScripts* sc = findInList(availableScripts, scriptName);
//			if (sc) {
//				removeInList(toSetup, scriptName);
//				removeInList(toRun, scriptName);
//				sc->state = LuaScriptState::unloaded;
//				return true;
//			}
//			return false;
//		}
//		bool load(QString scriptName) {
//			LuaScripts* sc = findInList(availableScripts, scriptName);
//			if (sc) {
//				sc->state = LuaScriptState::setup;
//				toSetup << sc;
//				return true;
//			}
//			return false;
//
//		}
//		bool setEnable(QString scriptName, bool enable) {
//			LuaScripts* sc = findInList(availableScripts, scriptName);
//			if (sc) {
//				removeInList(toSetup, scriptName);
//				removeInList(toRun, scriptName);
//				if (enable) {
//					toRun << sc;
//					sc->state = LuaScriptState::running;
//				}
//				return true;
//			}
//			return false;
//		}
//		bool restart(QString scriptName) {
//			LuaScripts* sc = findInList(availableScripts, scriptName);
//			if (sc) {
//				unload(scriptName);
//				load(scriptName);
//				return true;
//			}
//			return false;
//		}
//		void tick() {
//			for each (auto sc in toRun) {
//				sc->updateLuaVars();
//				sc->runFunction("Tick");
//				sc->updateEnvVars();
//			}
//			if (toSetup.size() != 0) {
//				for each (auto sc in toSetup) {
//					if (!findInList(toRun, sc->fileName)) {
//						sc->evalScript(sc->absfilePath);
//						sc->updateLuaVars();
//						sc->runFunction("Setup");
//						sc->updateEnvVars();
//						toRun.append(sc);
//						sc->state = LuaScriptState::running;
//					}
//				}
//				toSetup.clear();
//			}
//		}
//		LuaScripts* operator[](lua_State* lstate) {
//			return findInListLuaState(availableScripts, lstate);
//		}
//		LuaScripts* operator[](QString name) {
//			return findInList(availableScripts, name);
//		}
//	};
//
//	static int Save(lua_State* L) {
//		auto& v = VariableEnviroment::instance();
//		auto& l = LuaEnviroment::instance();
//		LuaScripts* lScript = l[L];
//		if (lScript) {
//			QString Args = lua_tostring(L, 1);
//			auto ls = Args.split(" ");
//			if (ls.size() == 2) {
//				if (v.isRegistred(ls[0])) {
//					if (ls[1].toInt() == 0) {
//						v.unPersist(ls[1]);
//					}
//					else {
//						Variable& var = v[ls[0]];
//						v.persist(var);
//					}
//				}
//			}
//		}
//		return 1;
//	}
//	static int Get(lua_State* L) {
//		auto& v = VariableEnviroment::instance();
//		auto& l = LuaEnviroment::instance();
//		LuaScripts* lScript = l[L];
//		if (lScript) {
//			QString varName = lua_tostring(L, 1);
//			if (v.isRegistred(varName)) {
//				Variable& target = v[varName];
//				lScript->binders.addVariable(target);
//			}
//		}
//		return 1;
//	}
//	static int Set(lua_State* L) {
//		auto& t = Types::instance();
//		auto& v = VariableEnviroment::instance();
//		auto& l = LuaEnviroment::instance();
//		LuaScripts* lScript = l[L];
//		if (lScript) {
//			QString Args = lua_tostring(L, 1);
//			auto ls = Args.split(" ");
//			if (ls.size() == 2) {
//				if (v.isRegistred(ls[0])) {
//					Variable& var = v[ls[0]];
//					var().set(var.type(), ls[1]);
//				}
//			}
//		}
//		return 1;
//	}
//	static int New(lua_State* L) {
//		auto& t = Types::instance();
//		auto& v = VariableEnviroment::instance();
//		auto& l = LuaEnviroment::instance();
//		LuaScripts* lScript = l[L];
//		if (lScript) {
//			QString Args = lua_tostring(L, 1);
//			auto ls = Args.split(" ");
//			if (ls.size() == 3) {
//				Variable* var = nullptr;
//				if (v.isRegistred(ls[1]))
//					var = &v[ls[1]];
//				else
//					var = &v.add(ls[1]);
//				(*var)().set(t.fromString(ls[0]), ls[2]);
//			}
//		}
//		return 1;
//	}
//	static int Del(lua_State* L) {
//		auto& t = Types::instance();
//		auto& v = VariableEnviroment::instance();
//		auto& l = LuaEnviroment::instance();
//		LuaScripts* lScript = l[L];
//		if (lScript) {
//			QString varname = lua_tostring(L, 1);
//			if (v.isRegistred(varname)) {
//				v.remove(varname);
//			}
//		}
//		return 1;
//	}
//};
#endif // !_lang
