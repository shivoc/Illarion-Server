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

#ifndef LUATRIGGERSCRIPT_HH
#define LUATRIGGERSCRIPT_HH

#include "globals.hpp"
#include "script/LuaScript.hpp"
#include "Item.hpp"

class Character;

class LuaTriggerScript : public LuaScript {
	public:
		LuaTriggerScript() { }
		LuaTriggerScript(const std::string& filename, const position&) { }
		LuaTriggerScript(const LuaTriggerScript&) = delete;
		LuaTriggerScript& operator=(const LuaTriggerScript&) = delete;

		void CharacterOnField(Character *Character) { }
		void MoveToField(Character *Character) { }
		void MoveFromField(Character *Character) { }
		void PutItemOnField(const ScriptItem &item, Character *Character) { }
		void TakeItemFromField(const ScriptItem &item, Character *Character) { }
};

#endif
