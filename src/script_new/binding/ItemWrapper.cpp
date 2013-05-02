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

#include "script/binding/ItemWrapper.hpp"

#include <lua.hpp>

#include "Item.hpp"

#include "script/binding/helper.hpp"

ItemWrapper::ItemWrapper() : Binder("Item") {
}

void ItemWrapper::setup_functions() {
	_functions.accessors["id"] = &getId;
	_functions.new_index["id"] = &setId;
	_functions.accessors["wear"] = &getWear;
	_functions.new_index["wear"] = &setWear;
	_functions.accessors["number"] = &getNumber;
	_functions.new_index["number"] = &setNumber;
	_functions.accessors["quality"] = &getQuality;
	_functions.new_index["quality"] = &setQuality;
	_functions.mem_fun["setData"] = &setData;
	_functions.mem_fun["getData"] = &getData;
	_functions.constructor = &create;
	_functions.gc = &gc_fun;
}

int ItemWrapper::gc_fun(struct lua_State* state) {
	Item* ptr = instance()->get(state, 1);
	lua_pop(state, 1);
	delete ptr;
	return 0;
}

int ItemWrapper::getId(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, itemptr->getId());
	return 1;
}

int ItemWrapper::setId(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	Item::id_type value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	itemptr->setId(value);
	return 0;
}

int ItemWrapper::getWear(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, itemptr->getWear());
	return 1;
}

int ItemWrapper::setWear(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	Item::id_type value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	itemptr->setWear(value);
	return 0;
}

int ItemWrapper::getNumber(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, itemptr->getNumber());
	return 1;
}

int ItemWrapper::setNumber(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	Item::id_type value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	itemptr->setNumber(value);
	return 0;
}

int ItemWrapper::getQuality(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	lua_pop(state, 1);
	lua_pushnumber(state, itemptr->getQuality());
	return 1;
}

int ItemWrapper::setQuality(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	Item::id_type value = luaL_checkint(state, 2);
	lua_pop(state, 2);
	itemptr->setQuality(value);
	return 0;
}

int ItemWrapper::create(lua_State* state) {
	int numargs = lua_gettop(state);

	switch (numargs) {
		case 0:
			instance()->push(state, Item());
			break;
		case 4:
			{
				Item::id_type id = luaL_checknumber(state, 1);
				Item::number_type number = luaL_checknumber(state, 2);
				Item::wear_type wear = luaL_checknumber(state, 3);
				Item::quality_type quality = luaL_checknumber(state, 4);
				instance()->push(state, Item(id, number, wear, quality));
				break;
			}
		default:
			return 0;
	}
	return 1;
}

int ItemWrapper::setData(lua_State* state) {
	auto itemptr = instance()->get(state, 1);

	std::string key = luaL_checkstring(state, 2);
	std::string value = luaL_checkstring(state, 3);
	lua_pop(state, 3);

	// TODO ignore int overload here?

	itemptr->setData(key, value);

	return 0;
}

int ItemWrapper::getData(lua_State* state) {
	auto itemptr = instance()->get(state, 1);
	std::string key = luaL_checkstring(state, 2);
	lua_pop(state, 2);

	lua_pushstring(state, itemptr->getData(key).c_str());

	return 1;
}
