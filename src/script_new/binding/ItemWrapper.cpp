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

