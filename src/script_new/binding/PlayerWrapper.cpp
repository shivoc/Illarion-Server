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

#include "script/binding/PlayerWrapper.hpp"

#include <lua.hpp>

#include "Player.hpp"
#include "World.hpp"

#include "script/LuaScript.hpp"

#include "script/binding/helper.hpp"

#define CLASS_TABLE_NAME "game.player"
#define CLASS_STATIC_TABLE_NAME "game.player.static"
#define CLASS_STATIC_TABLE_INSTANCE "game.player.static.instance"
#define CLASS_BASE_TABLE_NAME "game.character"

void PlayerWrapper::push(struct lua_State* state, Player* character) {
	TYPE_OF_CHARACTER_ID* charid = (TYPE_OF_CHARACTER_ID*)lua_newuserdata(state, sizeof(TYPE_OF_CHARACTER_ID*));
	*charid = character->getId();
	luaL_getmetatable(state, CLASS_TABLE_NAME);
	lua_setmetatable(state, -2);
}

int readonly_table(struct lua_State* state) {
        Logger::error(LogFacility::Script) << "tried to modify read-only table" << Log::end;
	return 0;
}

void PlayerWrapper::Register(struct lua_State* state) {
	luaH_register_class(state, CLASS_TABLE_NAME, CLASS_BASE_TABLE_NAME);

	luaL_newmetatable(state, CLASS_STATIC_TABLE_INSTANCE);
	lua_pushstring(state, "__index");

	luaL_newmetatable(state, CLASS_STATIC_TABLE_NAME);

	luaH_pushint(state, "english", static_cast<int>(Language::english));
	luaH_pushint(state, "german", static_cast<int>(Language::german));

	lua_settable(state, -3);

	lua_pushstring(state, "__newindex");
	lua_getglobal(state, "readonly");
	lua_settable(state, -3);

	luaL_getmetatable(state, CLASS_STATIC_TABLE_INSTANCE);
	lua_setmetatable(state, -2);

	lua_setglobal(state, "Player");
}

