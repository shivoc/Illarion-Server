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

LuaTestSupportScript::LuaTestSupportScript(const std::string &code, const std::string &scriptname)
    : LuaScript(code, scriptname) {
}

LuaTestSupportScript::~LuaTestSupportScript() noexcept {}

Item LuaTestSupportScript::item_test(const Item& in) {
        lua_getglobal(_luaState, "item_test");
	ItemWrapper::instance()->push(_luaState, in);

        int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return *ItemWrapper::instance()->get(_luaState, -1);
}

ScriptItem LuaTestSupportScript::scriptitem_test(const ScriptItem& in) {
        lua_getglobal(_luaState, "scriptitem_test");
	ScriptItemWrapper::instance()->push(_luaState, in);

        int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return *ScriptItemWrapper::instance()->get(_luaState, -1);
}

int LuaTestSupportScript::int_test(int arg) {
        lua_getglobal(_luaState, "int_test");
	lua_pushnumber(_luaState, arg);

        int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	return luaL_checknumber(_luaState, -1);
}

position LuaTestSupportScript::position_test(const position& pos) {
        lua_getglobal(_luaState, "position_test");
	PositionWrapper::instance()->push(_luaState, pos);

        int rc = lua_pcall(_luaState, 1, 1, 0);
	if (rc != 0) {
		logCurrentError();
	}

	auto ptr = PositionWrapper::instance()->get(_luaState, -1);
	return *ptr;
}
