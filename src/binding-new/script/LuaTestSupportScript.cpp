/*
 *  illarionserver - server for the game Illarion
 *  Copyright 2011 Illarion e.V.
 *
 *  This file is part of illarionserver.
 *
 *  illarionserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  illarionserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LuaTestSupportScript.hpp"

#include <lua.hpp>

#include "Item.hpp"
#include "binding/ItemWrapper.hpp"
#include "binding/ScriptItemWrapper.hpp"
#include "binding/PositionWrapper.hpp"
#include "World.hpp"
#include "Monster.hpp"

LuaTestSupportScript::LuaTestSupportScript(const std::string &code, const std::string &scriptname)
    : LuaScript(code, scriptname) {
}

LuaTestSupportScript::~LuaTestSupportScript() noexcept {}

template<>
Item LuaTestSupportScript::test(const Item& in) {
	lua_getglobal(_luaState, "test");
	ItemWrapper::instance()->push(_luaState, in);

	int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return *ItemWrapper::instance()->get(_luaState, -1);
}

template<>
ScriptItem LuaTestSupportScript::test(const ScriptItem& in) {
	lua_getglobal(_luaState, "test");
	ScriptItemWrapper::instance()->push(_luaState, in);

	int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return *ScriptItemWrapper::instance()->get(_luaState, -1);
}

template<>
int LuaTestSupportScript::test(const int& arg) {
	lua_getglobal(_luaState, "test");
	lua_pushnumber(_luaState, arg);

	int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return luaL_checknumber(_luaState, -1);
}

template<>
position LuaTestSupportScript::test(const position& pos) {
	lua_getglobal(_luaState, "test");
	PositionWrapper::instance()->push(_luaState, pos);

	int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	auto ptr = PositionWrapper::instance()->get(_luaState, -1);
	return *ptr;
}

template<>
WeatherStruct LuaTestSupportScript::test(const WeatherStruct&) {
	// TODO implement
	return {};
}

template<>
bool LuaTestSupportScript::test(Monster * const &) {
	// TODO implement
	return false;
}

template<>
int LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
std::string LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
bool LuaTestSupportScript::test(Monster* const&, const ScriptItem&) {
	// TODO implement
	return {};
}

template<>
bool LuaTestSupportScript::test(Monster* const&, Player* const&) {
	// TODO implement
	return {};
}

template<>
direction LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
unsigned LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
short unsigned LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
short LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
Character::face_to LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

template<>
ScriptItem LuaTestSupportScript::test(Monster* const&) {
	// TODO implement
	return {};
}

