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

#define CLASS_TABLE_NAME "game.item"

void ItemWrapper::push(struct lua_State* state, const Item& item) {
	lua_newtable(state);
	luaL_getmetatable(state, CLASS_TABLE_NAME);
	lua_setmetatable(state, -2);

	push_values(state, item);
}

void ItemWrapper::Register(struct lua_State* state) {
	luaH_register_struct(state, CLASS_TABLE_NAME);
}

void ItemWrapper::push_values(struct lua_State* state, const Item& item) {
	luaH_pushint(state, "id", item.getId());
}
