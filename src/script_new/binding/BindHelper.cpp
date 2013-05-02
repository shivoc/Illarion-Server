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

#include "BindHelper.hpp"

#include <lua.hpp>

#include "Logger.hpp"

std::vector<std::function<void()>> BindHelper::_deregister_functions;

lua_function get_function(const function_map& map, std::string method) {
	auto target_fun = map.find(method);
	if (target_fun != map.end()) {
		return target_fun->second;
	}

	return nullptr;
}

int BindHelper::resolve_index(lua_State* state, const BindFunctions& functions) {
	const char* method = luaL_checkstring(state, 2);

	lua_remove(state, 2);

	
	lua_function target_fun = nullptr;
	const BindFunctions* ptr = &functions;

	do {
		target_fun = get_function(ptr->mem_fun, method);
		ptr = ptr->parent;
	} while (target_fun == nullptr && ptr != nullptr);

	if (target_fun != nullptr) {
		lua_pushcfunction(state, target_fun);
	} else {
		ptr = &functions;
		do {
			target_fun = get_function(ptr->accessors, method);
			ptr = ptr->parent;
		} while (target_fun == nullptr && ptr != nullptr);

		if (target_fun != nullptr) {
			return target_fun(state);
		} else {
			Logger::error(LogFacility::Script) << "function " << method << " not found" << Log::end;
			lua_pushnil(state);
		}
	}

	return 1;
}

int BindHelper::new_index(lua_State* state, const BindFunctions& functions) {
	const char* method = luaL_checkstring(state, 2);
	lua_remove(state, 2);

	lua_function target_fun = nullptr;
	const BindFunctions* ptr = &functions;

	do {
		target_fun = get_function(ptr->new_index, method);
		ptr = ptr->parent;
	} while (target_fun == nullptr && ptr != nullptr);

	if (target_fun != nullptr) {
		return target_fun(state);
	} else {
		Logger::error(LogFacility::Script) << "function " << method << " not found" << Log::end;
	}

	lua_pop(state, lua_gettop(state));

	return 0;
}

int BindHelper::resolve_static(lua_State* state, const BindFunctions& functions) {
	const char* method = luaL_checkstring(state, 2);
	lua_remove(state, 2);

	lua_function target_fun = nullptr;
	const BindFunctions* ptr = &functions;

	do {
		target_fun = get_function(ptr->static_methods, method);
		ptr = ptr->parent;
	} while (target_fun == nullptr && ptr != nullptr);

	if (target_fun != nullptr) {
		lua_pushcfunction(state, target_fun);
	} else {
		ptr = &functions;
		do {
			target_fun = get_function(ptr->static_data, method);
			ptr = ptr->parent;
		} while (target_fun == nullptr && ptr != nullptr);

		if (target_fun != nullptr) {
			return target_fun(state);
		} else {
			Logger::error(LogFacility::Script) << "function " << method << " not found" << Log::end;
			lua_pushnil(state);
		}
	}

	return 1;
}

void BindHelper::Register(lua_State* state, const std::string& classname, lua_function index, lua_function newindex, const BindFunctions& functions) {
	std::string tablename = "game." + classname;
	luaL_newmetatable(state, tablename.c_str());

	lua_pushstring(state, "__index");
	lua_pushcfunction(state, index);
	lua_settable(state, -3);

	lua_pushstring(state, "__newindex");
	lua_pushcfunction(state, newindex);
	lua_settable(state, -3);

	if (functions.gc != nullptr) {
		lua_pushstring(state, "__gc");
		lua_pushcfunction(state, functions.gc);
		lua_settable(state, -3);
	}

	if (functions.to_string != nullptr) {
		lua_pushstring(state, "__tostring");
		lua_pushcfunction(state, functions.to_string);
		lua_settable(state, -3);
	}

	if (functions.eq != nullptr) {
		lua_pushstring(state, "__eq");
		lua_pushcfunction(state, functions.eq);
		lua_settable(state, -3);
	}

	lua_pop(state, 1);

	if (functions.constructor != nullptr) {
		lua_pushcfunction(state, functions.constructor);
		lua_setglobal(state, classname.c_str());
	}
}

void BindHelper::RegisterGlobal(lua_State* state, const std::string& name, lua_function index) {
	std::string tablename = "static." + name;
	luaL_newmetatable(state, tablename.c_str());

	lua_pushstring(state, "__index");
	lua_pushcfunction(state, index);
	lua_settable(state, -3);

        luaL_getmetatable(state, tablename.c_str());
        lua_setmetatable(state, -2);

	lua_setglobal(state, name.c_str());
}

void BindHelper::push_ptr_userdata(lua_State* state, void* ptr, size_t size, const std::string& classname) {
	std::string tablename = "game." + classname;
	void** newptr = (void**)lua_newuserdata(state, size);
	*newptr = ptr;

	luaL_getmetatable(state, tablename.c_str());
	lua_setmetatable(state, -2);
}

void** BindHelper::get_ptr_userdata(lua_State* state, int index, const std::string& classname) {
	if (!lua_isuserdata(state, index)) {
		arg_error(state, index, classname);
		return nullptr;
	}

	void** raw_ptr = (void**)lua_touserdata(state, index);
	return raw_ptr;
}

void BindHelper::arg_error(lua_State* state, int index, const std::string& classname) {
	std::string error = "expected " + classname;
	luaL_argcheck(state, false, index, error.c_str());
}

void BindHelper::numarg_error(lua_State* state, const std::string& error) {
	luaL_argcheck(state, false, 0, error.c_str());
}

void BindHelper::push_id(lua_State* state, uint64_t id, const std::string& classname) {
	std::string tablename = "game." + classname;
	uint64_t* id_ptr = (uint64_t*)lua_newuserdata(state, sizeof(uint64_t));
	*id_ptr = id;

	luaL_getmetatable(state, tablename.c_str());
	lua_setmetatable(state, -2);
}

uint64_t BindHelper::get_id(lua_State* state, int index, const std::string& classname) {
	if (!lua_isuserdata(state, index)) {
		arg_error(state, index, classname);
		return -1;
	}

	uint64_t* id_ptr = (uint64_t*)lua_touserdata(state, index);
	return *id_ptr;
}

int BindHelper::push_int(lua_State* state, uint64_t value) {
	lua_pushnumber(state, value);
	return 1;
}

void BindHelper::unregister() {
	for (auto func : _deregister_functions)
		func();
}

void BindHelper::add_deregister_function(std::function<void()> func) {
	_deregister_functions.push_back(func);
}
