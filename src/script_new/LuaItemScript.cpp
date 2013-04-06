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

#include "LuaItemScript.hpp"

#include <lua.hpp>

#include "binding/CharacterWrapper.hpp"
#include "binding/ScriptItemWrapper.hpp"

std::string luarc_tostring(int rc);

LuaItemScript::LuaItemScript(const std::string &code, const std::string &codename, const CommonStruct &comstr) : LuaScript(code, codename) {
}

// "User:inform(\"You have used the item with the name '\"..world:getItemName(SourceItem.id,Player.english)..\"' and the ID \"..tonumber(SourceItem.id)..\".\")\n"

void LuaItemScript::UseItem(Character* user, const ScriptItem& sourceItem, unsigned char ltastate) {
        lua_getglobal(_luaState, "UseItem");
	CharacterWrapper::push(_luaState, user);
	ScriptItemWrapper::push(_luaState, sourceItem);
	lua_pushnumber(_luaState, ltastate);
        int rc = lua_pcall(_luaState, 3, 0, 0);
	if (rc != 0) {
		logCurrentError();
	}
}

LuaItemScript::LuaItemScript() {
}

LuaItemScript::LuaItemScript(const std::string& filename, const CommonStruct &comstr) {
}

bool LuaItemScript::actionDisturbed(Character *performer, Character *disturber) {
	return false;
}

void LuaItemScript::LookAtItem(Character *who, const ScriptItem &item) {
        lua_getglobal(_luaState, "LookAtItem");
	CharacterWrapper::push(_luaState, who);
	ScriptItemWrapper::push(_luaState, item);
        int rc = lua_pcall(_luaState, 2, 0, 0);
	if (rc != 0) {
		logCurrentError();
	}
/*
    LuaItemScript script {"function LookAtItem(player, item)\n"
                          "local lookAt = ItemLookAt()\n"
                          "lookAt.name = world:getItemName(item.id,Player.english)\n"
                          "world:itemInform(player, item, lookAt)\n"
                          "return true\n"
                          "end",
                          "lookat_test", itemdef
                         };
*/
}

bool LuaItemScript::MoveItemBeforeMove(Character *who, const ScriptItem &sourceItem, const ScriptItem &targetItem) {
	return false;
}

void LuaItemScript::MoveItemAfterMove(Character *who, const ScriptItem &sourceItem, const ScriptItem &targetItem) {
}

void LuaItemScript::CharacterOnField(Character *who) {
}
