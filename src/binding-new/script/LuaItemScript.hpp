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

#ifndef LUAITEMSCRIPT_HH
#define LUAITEMSCRIPT_HH

#include "TableStructs.hpp"
#include "Item.hpp"

#include "script/LuaScript.hpp"

class Character;

class LuaItemScript : public LuaScript {
	public:
		LuaItemScript();
		LuaItemScript(const std::string& filename, const CommonStruct &comstr);
		LuaItemScript(const std::string &code, const std::string &codename, const CommonStruct &comstr);
		LuaItemScript(const LuaItemScript&) = delete;
		LuaItemScript& operator=(const LuaItemScript&) = delete;

		void UseItem(Character* user, const ScriptItem& sourceItem, unsigned char ltastate);
		bool actionDisturbed(Character *performer, Character *disturber);
		void LookAtItem(Character *who, const ScriptItem &t_item);
		bool MoveItemBeforeMove(Character *who, const ScriptItem &sourceItem, const ScriptItem &targetItem);
		void MoveItemAfterMove(Character *who, const ScriptItem &sourceItem, const ScriptItem &targetItem);
		void CharacterOnField(Character *who);
};

#endif
