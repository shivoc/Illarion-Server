/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU  General  Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU  General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#include "script/binding/helper.hpp"

#include <lua.hpp>

#include "script/LuaScript.hpp"

#define METHODS ".methods"
#define NEWMETHODS ".newmethods"

Character::informType check_informtype(int informtype) {
	Character::informType type = static_cast<Character::informType>(informtype);
	// no default switch to ensure compile error on change to enum
	switch (type) {
		case Character::informServer:
		case Character::informBroadcast:
		case Character::informGM:
		case Character::informScriptLowPriority:
		case Character::informScriptMediumPriority:
		case Character::informScriptHighPriority:
			return type;
	}

	return Character::informScriptMediumPriority;
}

void luaH_register_struct(struct lua_State* state, const char* tablename, const char* basename) {
	// we only handle copy by value, no methods struct here...
	// TODO we might need to change this if we want to inherit methods, have to find a way to handle getter/setter of base classes without preventing setting table values
	luaL_newmetatable(state, tablename);
	lua_pop(state, 1);
}

void luaH_register_class(struct lua_State* state, const char* tablename, const char* basename) {
	// TODO check if getter/setter of base class are correctly used
	luaL_newmetatable(state, tablename);

	lua_pushstring(state, "__index");

	// create new metatable for class methods
	std::string methods_table = tablename;
	methods_table += METHODS;
	luaL_newmetatable(state, methods_table.c_str());

	if (basename != nullptr) {
		// set item class table as index for inheritance
		lua_pushstring(state, "__index");
		luaL_getmetatable(state, basename);
		lua_settable(state, -3);
	}

	lua_settable(state, -3);  /* metatable.__index = metatable */

	// create new metatable for write methods
	std::string newmethods_table = tablename;
	newmethods_table += NEWMETHODS;
	lua_pushstring(state, "__newindex");
	luaL_newmetatable(state, newmethods_table.c_str());

	if (basename != nullptr) {
		// set item class table as index for inheritance
		lua_pushstring(state, "__newindex");
		luaL_getmetatable(state, basename);
		lua_settable(state, -3);
	}

	lua_settable(state, -3);


//	luaL_getmetatable(state, tablename);
//	lua_setmetatable(state, -2);

	lua_pop(state, 1);
}

void luaH_getmethods(struct lua_State* state, const char* tablename) {
	std::string methods_table = tablename;
	methods_table += METHODS;
	luaL_getmetatable(state, methods_table.c_str());
}

void luaH_setmethods(struct lua_State* state, const char* tablename) {
	std::string newmethods_table = tablename;
	newmethods_table += NEWMETHODS;
	luaL_getmetatable(state, newmethods_table.c_str());
}

void luaH_register_function(struct lua_State* state, const char* name, lua_function func) {
	lua_pushstring(state, name);
	lua_pushcfunction(state, func);
	lua_settable(state, -3);
}

void luaH_pushint(lua_State *l, const std::string &key, const int &value) {
    lua_pushstring(l, key.c_str());
    lua_pushnumber(l, value);
    lua_settable(l, -3);
}

std::shared_ptr<script_data_exchangemap> luaH_convert_to_map(struct lua_State* state, int index) {
	assert(index > 0);

	int type = lua_type(state, index);
	if (type == LUA_TNIL) {
		return {};
	}

	if (type != LUA_TTABLE) {
		throw ScriptException("Usage of invalid data map type. Data maps must be tables or nil.");
	}

	auto result = std::make_shared<script_data_exchangemap>();

	lua_pushnil(state);
	while (lua_next(state, index)) {
		if (!lua_isstring(state, -2)) {
			throw ScriptException("Usage of invalid data map key. Data map keys must be strings.");
		}
		std::string key = lua_tolstring(state, -2, NULL);
			
		int valuetype = lua_type(state, -1);
		std::string value;
		switch (valuetype) {
			case LUA_TSTRING:
				value = lua_tolstring(state, -1, NULL);
				break;
			case LUA_TNUMBER:
				value = std::to_string(lua_tonumber(state, -1));
				break;
			default:
				throw ScriptException("Usage of invalid data map value. Data map values must be numbers or strings.");
		}

		result->push_back(std::make_pair(key, value));
		lua_pop(state, 1);
	}

	return result;
}

