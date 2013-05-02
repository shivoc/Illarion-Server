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

#include "LuaScript.hpp"

#include <lua.hpp>

#include "Logger.hpp"

#include "binding/CharacterWrapper.hpp"
#include "binding/PlayerWrapper.hpp"
#include "binding/WorldWrapper.hpp"
#include "binding/PositionWrapper.hpp"
#include "binding/ItemWrapper.hpp"
#include "binding/ScriptItemWrapper.hpp"
#include "binding/ItemLookAtWrapper.hpp"
#include "binding/ContainerWrapper.hpp"

struct lua_State* LuaScript::_luaState = nullptr;

std::string luarc_tostring(int rc) {
	switch (rc) {
		case LUA_ERRFILE:
			return "could not access file";
		case LUA_ERRSYNTAX:
			return "syntax error";
		case LUA_ERRMEM:
			return "insufficient memory";
		default:
			return "unhandled rc: " + std::to_string(rc);
	}
}

LuaScript::LuaScript(const std::string &code, const std::string &codename) {
	if (_luaState == nullptr)
		init_state();

	int rc = luaL_loadbuffer(_luaState, code.c_str(), code.length(), codename.c_str());
	if (rc != 0) {
		throw ScriptException("Error in loading script: " + luarc_tostring(rc));
	}
        rc = lua_pcall(_luaState, 0, 0, 0);
	if (rc != 0) {
		throw ScriptException("Error in loading script: " + luarc_tostring(rc));
	}
}

void LuaScript::init_state() {
        _luaState = lua_open();
	luaL_openlibs(_luaState);

	std::string readonly = "function readonly(tab, name, value)\nerror(\"readonly table\")\nend";
	int rc = luaL_loadbuffer(_luaState, readonly.c_str(), readonly.length(), "readonly function");
	if (rc != 0) {
		throw ScriptException("Error in loading script: " + luarc_tostring(rc));
	}

        rc = lua_pcall(_luaState, 0, 0, 0);
	if (rc != 0) {
		throw ScriptException("Error in loading script: " + luarc_tostring(rc));
	}

	CharacterWrapper::instance()->Register(_luaState);
	PlayerWrapper::instance()->Register(_luaState);
	WorldWrapper::instance()->Register(_luaState);
	PositionWrapper::instance()->Register(_luaState);
	ItemWrapper::instance()->Register(_luaState);
	ScriptItemWrapper::instance()->Register(_luaState);
	ItemLookAtWrapper::instance()->Register(_luaState);
	ContainerWrapper::instance()->Register(_luaState);
}

void LuaScript::logCurrentError() {
    const char *cerr = lua_tostring(_luaState, -1);
    lua_pop(_luaState, 1);

    std::string err;

    if (cerr) {
        err = cerr;
    } else {
        err = "UNKNOWN ERROR, CONTACT SERVER DEVELOPER";
        lua_pushstring(_luaState, err.c_str());
        add_backtrace(_luaState);
        err = lua_tostring(_luaState, -1);
        lua_pop(_luaState, 1);
    }

    if (err.length() > 1) {
        Logger::error(LogFacility::Script) << err << Log::end;
    }
}

int LuaScript::add_backtrace(lua_State *L) {
    lua_Debug d;
    std::stringstream msg;

    if (lua_tostring(L, -1) != NULL) {
        std::string err = lua_tostring(L, -1);
        lua_pop(L, 1);
        msg << err << std::endl;
    }

    int level = 0;

    while (lua_getstack(L, ++level, &d)) {
        lua_getinfo(L, "Sln", &d);
        msg << "#" << level << " called by: " << d.short_src << ":" << d.currentline;

        if (d.name != 0) {
            msg << "(" << d.namewhat << " " << d.name << ")";
        }

        msg << std::endl;
    }

    if (level == 1) { // do not mind if an entry point is missing
        std::string empty = "";
        lua_pushstring(L, empty.c_str());
    } else {
        lua_pushstring(L, msg.str().c_str());
    }

    return 1;
}

void LuaScript::shutdownLua() {
	if (_luaState != nullptr) {
		std::cout << "calling lua_close" << std::endl;
		lua_close(_luaState);
		_luaState = nullptr;

		BindHelper::unregister();
	}
}

