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

#ifndef LUAMAGICSCRIPT_HH
#define LUAMAGICSCRIPT_HH

#include "script/LuaScript.hpp"

struct SpellStruct;
class Character;

class LuaMagicScript : public LuaScript {
	public:
		LuaMagicScript(const std::string& filename, const SpellStruct&) { }
		LuaMagicScript(const LuaMagicScript&) = delete;
		LuaMagicScript& operator=(const LuaMagicScript&) = delete;

		void CastMagic(Character *caster, unsigned char ltastate) { }
		void CastMagicOnItem(Character *caster, const ScriptItem &TargetItem, unsigned char ltastate) { }
		void CastMagicOnCharacter(Character *caster, Character *target, unsigned char ltastate) { }
		void CastMagicOnField(Character *caster, const position &pos, unsigned char ltastate) { }
		bool actionDisturbed(Character *performer, Character *disturber) { return false; }
};

#endif
