#include "LuaScripting.h"
#include "Definitions.h"
#include "ComponentStorage.h"
#include "ResourcesMgr.h"
#include "BehaviorLua.h"
#include <LuaBridge.h>

using namespace luabridge;

namespace Cog {

	void LuaScripting::OnInit() {

		COGLOGDEBUG("Lua", "Initializing lua binding");

		L = luaL_newstate();
		luaL_openlibs(L);

		LuaMapper mapper;
		mapper.InitLuaMapping(L);

		LoadAllScripts();
	}

	BehaviorLua* LuaScripting::CreateLuaBehavior(StrId key) {

		if (behaviorPrototypes.find(key) == behaviorPrototypes.end()) {
			return nullptr;
		}

		int reference = behaviorPrototypes.find(key)->second;

		lua_rawgeti(L, LUA_REGISTRYINDEX, reference);
		LuaRef ref = LuaRef::fromStack(L, lua_gettop(L));
		COGASSERT(!ref.isNil(), "Lua", "Wrong lua object; expected reference");
		auto newCpp = ref["NewCpp"];
		COGASSERT(!newCpp.isNil(), "Lua", "Wrong lua object; expected reference to method NewCpp");
		
		auto behLua = new BehaviorLua();
		newCpp(ref, behLua);


		return behLua;
	}

	int LuaScripting::RegisterBehaviorPrototype(luabridge::lua_State* L) {
		COGASSERT(lua_gettop(L) == 2, "Lua", "Wrong registration call! Expected two parameters: registered object and its name");

		// get behavior name and register this object
		string name = lua_tostring(L, 2);

		// remove string parameter and save only the object
		lua_pop(L, 1);

		int r = luaL_ref(L, LUA_REGISTRYINDEX);

		// store reference
		behaviorPrototypes[StrId(name)] = r;
		return 0; // number of return values
	}

	void LuaScripting::LoadScript(ofFile file) {

		string name = file.getFileName();

		COGLOGDEBUG("Lua", "Loading script %s", name.c_str());

		int status = luaL_loadfile(L, file.getAbsolutePath().c_str());

		if (status != 0) {
			CogLogError("Lua", lua_tostring(L, -1));
		}
		else {
			// run script
			status = lua_pcall(L, 0, LUA_MULTRET, 0);
			if (status != 0) {
				CogLogError("Lua", lua_tostring(L, -1));
			}
		}
	}

	void LuaScripting::LoadScript(string str) {

		COGLOGDEBUG("Lua", "Loading script from string");

		int status = luaL_loadstring(L, str.c_str());
		if (status != 0) {
			CogLogError("Lua", lua_tostring(L, -1));
		}
		else {
			// execute program
			status = lua_pcall(L, 0, LUA_MULTRET, 0);
			if (status != 0) {
				CogLogError("Lua", lua_tostring(L, -1));
			}
		}
	}

	void LuaScripting::LoadAllScripts() {
		
		COGLOGDEBUG("Lua", "Loading scripts");

		ofDirectory baseDir(PATH_BASE_SCRIPTS);

		if (baseDir.exists()) {
			auto files = baseDir.getFiles();
			for (auto file : files) {
				LoadScript(file);
			}
		}

		ofDirectory dir(PATH_SCRIPTS);

		if (dir.exists()) {
			auto files = dir.getFiles();

			for (auto file : files) {
				LoadScript(file);
			}
		}
	}


} // namespace