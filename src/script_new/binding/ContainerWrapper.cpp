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

#include "ContainerWrapper.hpp"

#include <lua.hpp>

#include "Container.hpp"

#include "script/LuaScript.hpp"

#include "script/binding/helper.hpp"

#define CLASS_TABLE_NAME "game.container"

void ContainerWrapper::Register(struct lua_State* state) {
	luaH_register_class(state, CLASS_TABLE_NAME);

	luaH_getmethods(state, CLASS_TABLE_NAME);

	luaH_register_function(state, "countItem", countItem);
	luaH_register_function(state, "eraseItem", eraseItem);

	lua_pop(state, 1);
}

void ContainerWrapper::push(struct lua_State* state, Container* container) {
	Container** containerp = (Container**)lua_newuserdata(state, sizeof(Container**));
	*containerp = container;
	luaL_getmetatable(state, CLASS_TABLE_NAME);
	lua_setmetatable(state, -2);
}

Container* ContainerWrapper::get(struct lua_State* state, int index) {
	Container** containerp = (Container**)luaL_checkudata(state, index, CLASS_TABLE_NAME);
	return *containerp;
}

int ContainerWrapper::countItem(struct lua_State* state) {
	auto numargs = lua_gettop(state);

	if (numargs > 3 || numargs < 2) {
		// TODO log
		throw ScriptException("wrong number or arguments");
	}

	Container** containerp = (Container**)luaL_checkudata(state, 1, CLASS_TABLE_NAME);
	
	Item::id_type itemid = luaL_checknumber(state, 2);

	std::shared_ptr<script_data_exchangemap> data;
	if (numargs == 3) {
		data = luaH_convert_to_map(state, 3);
	}

	int result;
	if (numargs == 2) {
		result = (*containerp)->countItem(itemid);
	} else {
		result = (*containerp)->countItem(itemid, data.get());
	}

	lua_pop(state, numargs);
	lua_pushnumber(state, result);

	return 1;
}

// TYPE_OF_ITEM_ID, int, const luabind::object &
int ContainerWrapper::eraseItem(struct lua_State* state) {
	auto numargs = lua_gettop(state);

	if (numargs > 4 || numargs < 3) {
		// TODO log
		throw ScriptException("wrong number or arguments");
	}

	Container** containerp = (Container**)luaL_checkudata(state, 1, CLASS_TABLE_NAME);
	
	Item::id_type itemid = luaL_checknumber(state, 2);
	Item::number_type number = luaL_checknumber(state, 3);

	std::shared_ptr<script_data_exchangemap> data;
	if (numargs == 4) {
		data = luaH_convert_to_map(state, 4);
	}

	int result;
	if (numargs == 3) {
		result = (*containerp)->eraseItem(itemid, number);
	} else {
		result = (*containerp)->eraseItem(itemid, number, data.get());
	}

	lua_pop(state, numargs);
	lua_pushnumber(state, result);

	return 1;
}

