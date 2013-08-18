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

WorldWrapper::WorldWrapper() : Binder("world") {
}

void WorldBinder::push(lua_State* state, const World& target, const std::string& classname) {
	BindHelper::push_id(state, 0, classname);
}

World* WorldBinder::get(lua_State* state, int index, const std::string& classname) {
	return World::get();
}

void WorldWrapper::setup_functions() {
	_functions.static_methods["getItemName"] = &getItemName;
	_functions.static_methods["itemInform"] = &itemInform;
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

