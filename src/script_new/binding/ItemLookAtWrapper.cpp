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

#include "script/binding/ItemLookAtWrapper.hpp"

#include <lua.hpp>

#include "Item.hpp"

#include "script/LuaScript.hpp"

#include "script/binding/helper.hpp"

ItemLookAtWrapper* ItemLookAtWrapper::_instance;

ItemLookAtWrapper::ItemLookAtWrapper() : Binder("ItemLookAt") {
}

void ItemLookAtWrapper::setup_functions() {
	_functions.accessors["name"] = &getName;
	_functions.new_index["name"] = &setName;
	_functions.constructor = &create;
}

int ItemLookAtWrapper::create(struct lua_State* state) {
	instance()->push(state, *(new ItemLookAt()));
	return 1;
}

int ItemLookAtWrapper::getName(struct lua_State* state) {
	ItemLookAt* itemlookat = instance()->get(state, 1);
	if (lua_gettop(state) > 1) {
		// TODO log error
		throw ScriptException("wrong number of arguments");
	}

	lua_pop(state, 1);

	lua_pushstring(state, itemlookat->getName().c_str());

	return 1;
}

int ItemLookAtWrapper::setName(struct lua_State* state) {
	ItemLookAt* itemlookat = instance()->get(state, 1);
	if (lua_gettop(state) != 2) {
		// TODO log error
		throw ScriptException("wrong number of arguments");
	}

	const char* name = luaL_checkstring(state, 2);
	itemlookat->setName(name);

	lua_pop(state, 2);

	return 0;
}
