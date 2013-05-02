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

#include "script/binding/PositionWrapper.hpp"

#include <lua.hpp>

#include "globals.hpp"

PositionWrapper::PositionWrapper() : Binder("position") {
}

void PositionWrapper::setup_functions() {
	_functions.accessors["x"] = &get_x;
	_functions.new_index["x"] = &set_x;
	_functions.accessors["y"] = &get_y;
	_functions.new_index["y"] = &set_y;
	_functions.accessors["z"] = &get_z;
	_functions.new_index["z"] = &set_z;
	_functions.constructor = &new_pos;
	_functions.gc = &gc_fun;
	_functions.to_string = &to_string;
	_functions.eq = &equal;
}

int PositionWrapper::new_pos(struct lua_State* state) {
	if (lua_gettop(state) == 0) {
		lua_pop(state, 1);
		instance()->push(state, position());
	} else if (lua_gettop(state) == 3) {
		short x = luaL_checknumber(state, 1);
		short y = luaL_checknumber(state, 2);
		short z = luaL_checknumber(state, 3);
		lua_pop(state, 3);
		instance()->push(state, position(x,y,z));
	} else {
		BindHelper::numarg_error(state, "position() expects 0 or 3 arguments");
		return 0;
	}
	return 1;
}

int PositionWrapper::get_x(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, ptr->x);
	return 1;
}

int PositionWrapper::set_x(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	short int value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	ptr->x = value;
	return 0;
}

int PositionWrapper::get_y(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, ptr->y);
	return 1;
}

int PositionWrapper::set_y(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	short int value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	ptr->y = value;
	return 0;
}

int PositionWrapper::get_z(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, ptr->z);
	return 1;
}

int PositionWrapper::set_z(lua_State* state) {
	auto ptr = instance()->get(state, 1);
	short int value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	ptr->z = value;
	return 0;
}

int PositionWrapper::gc_fun(struct lua_State* state) {
	position* ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	delete ptr;
	return 0;
}

int PositionWrapper::to_string(lua_State* state) {
	position* ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	
	lua_pushstring(state, ptr->toString().c_str());
	return 1;
}

int PositionWrapper::equal(lua_State* state) {
	position* ptr1 = instance()->get(state, 1);
	position* ptr2 = instance()->get(state, 2);
	lua_pop(state, 2);
	
	bool result = (*ptr1 == *ptr2);

	lua_pushboolean(state, result);

	return 1;
}

