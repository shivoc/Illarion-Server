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

#include "script/binding/WorldWrapper.hpp"

#include <lua.hpp>

#include "World.hpp"

#include "script/binding/helper.hpp"
#include "script/binding/CharacterWrapper.hpp"
#include "script/binding/ScriptItemWrapper.hpp"
#include "script/binding/ItemLookAtWrapper.hpp"

#define CLASS_TABLE_NAME "game.world"

void WorldWrapper::push(struct lua_State* state, World* world) {
	luaH_getmethods(state, CLASS_TABLE_NAME);
}

void WorldWrapper::Register(struct lua_State* state) {
	// we only use the methods table for this class, doesn't seem to work if we push the class table...
	// TODO check if everything works as expected...
	luaH_register_class(state, CLASS_TABLE_NAME);

	luaH_getmethods(state, CLASS_TABLE_NAME);
	luaH_register_function(state, "getItemName", getItemName);
	luaH_register_function(state, "itemInform", itemInform);

	lua_setglobal(state, "world");
}

int WorldWrapper::getItemName(struct lua_State* state) {
	TYPE_OF_ITEM_ID itemid = luaL_checkint(state, -2);
	uint8_t language = luaL_checkint(state, -1);

	lua_pop(state, lua_gettop(state));

	auto result = World::get()->getItemName(itemid, language);
	lua_pushstring(state, result.c_str());

	return 1;
}

int WorldWrapper::itemInform(struct lua_State* state) {
	Character* character = CharacterWrapper::instance()->get(state, -3);
	ScriptItem scriptitem = *ScriptItemWrapper::instance()->get(state, -2);
	ItemLookAt itemlookat = *ItemLookAtWrapper::instance()->get(state, -1);

	World::get()->itemInform(character, scriptitem, itemlookat);

	lua_pop(state, lua_gettop(state));

	return 0;
}

