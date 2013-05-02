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

#include "script/binding/ScriptItemWrapper.hpp"

#include <lua.hpp>

#include "Item.hpp"

#include "script/binding/CharacterWrapper.hpp"
#include "script/binding/PositionWrapper.hpp"
#include "script/binding/ContainerWrapper.hpp"

ScriptItemWrapper::ScriptItemWrapper() : Binder("scriptItem") {
}

void ScriptItemWrapper::setup_functions() {
	_functions.constructor = &create;
	_functions.gc = &gc_fun;

	_functions.mem_fun["getType"] = &getType;
	_functions.accessors["owner"] = &getOwner;
	_functions.accessors["pos"] = &getPosition;
	_functions.accessors["itempos"] = &getItemposition;
	_functions.accessors["inside"] = &getInside;

	_functions.static_data["notdefined"] = ValueBinder<static_cast<int>(ScriptItem::notdefined)>;
	_functions.static_data["field"] = ValueBinder<static_cast<int>(ScriptItem::it_field)>;
	_functions.static_data["inventory"] = ValueBinder<static_cast<int>(ScriptItem::it_inventory)>;
	_functions.static_data["belt"] = ValueBinder<static_cast<int>(ScriptItem::it_belt)>;
	_functions.static_data["container"] = ValueBinder<static_cast<int>(ScriptItem::it_container)>;
}

int ScriptItemWrapper::create(lua_State* state) {
	instance()->push(state, ScriptItem());
	return 1;
}

int ScriptItemWrapper::gc_fun(struct lua_State* state) {
	std::cout << "gc scriptitem " << lua_gettop(state) << std::endl;
	ScriptItem* ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	delete ptr;
	return 0;
}

int ScriptItemWrapper::getOwner(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	if (itemptr->owner == nullptr) {
		lua_pushnil(state);
	} else {
		CharacterWrapper::instance()->push(state, *(itemptr->owner));
	}
	return 1;
}

int ScriptItemWrapper::getPosition(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	PositionWrapper::instance()->push(state, itemptr->pos);
	return 1;
}

int ScriptItemWrapper::getItemposition(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pushnumber(state, itemptr->itempos);
	return 1;
}

int ScriptItemWrapper::getInside(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	if (itemptr->inside == nullptr) {
		lua_pushnil(state);
	} else {
		ContainerWrapper::instance()->push(state, *(itemptr->inside));
	}
	return 1;
}

int ScriptItemWrapper::getType(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pushnumber(state, itemptr->getType());
	return 1;
}

