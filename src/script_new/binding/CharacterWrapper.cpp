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

#include "script/binding/CharacterWrapper.hpp"

#include <lua.hpp>

#include "Character.hpp"
#include "World.hpp"

#include "script/LuaScript.hpp"

#include "script/binding/helper.hpp"
#include "script/binding/ContainerWrapper.hpp"

#define CLASS_TABLE_NAME "game.character"

Character* CharIdBinder::getById(uint64_t id) {
	return World::get()->findCharacter(id);
}

uint64_t CharIdBinder::getId(Character const* character) {
	return character->getId();
}

void CharacterWrapper::setup_functions() {
	_functions.mem_fun["inform"] = &inform;
	_functions.mem_fun["getBackPack"] = &getBackPack;
}

CharacterWrapper::CharacterWrapper() : Binder("Character") {
}

int CharacterWrapper::inform(struct lua_State* state) {
	Character* character = instance()->get(state, 1);

	std::string arg1 = luaL_checkstring(state, 2);
	std::string arg2;
	Character::informType informtype = Character::informScriptMediumPriority;
	
	int num_args = lua_gettop(state);

	int arg = 3;
	if (num_args >= arg && lua_type(state, arg) == LUA_TSTRING) {
		arg2 = luaL_checkstring(state, arg);
		++arg;
	}

	if (num_args >= arg && lua_type(state, arg) == LUA_TNUMBER) {
		informtype = check_informtype(luaL_checknumber(state, arg));
	}

	if (num_args >= arg) {
		// to many arguments for function
		// TODO inform of error
		throw ScriptException("error in arguments");
	}

	if (character != nullptr) {
		if (arg2.empty()) {
			character->inform(arg1, informtype);
		} else {
			character->inform(arg1, arg2, informtype);
		}
	} else {
		// TODO log error
		throw ScriptException("char not found");
	}

	lua_pop(state, num_args);

	return 0;
}

int CharacterWrapper::getBackPack(struct lua_State* state) {
	Character* character = instance()->get(state, 1);
	
	if (character == nullptr) {
		// TODO log error
		throw ScriptException("char not found");
	}

	if (lua_gettop(state) > 1) {
		return luaL_argerror(state, 2, "invalid extra arguments");
	}

	ContainerWrapper::instance()->push(state, *(character->GetBackPack()));

	return 1;
}

