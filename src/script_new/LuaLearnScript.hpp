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

#ifndef LUALEARNSCRIPT_HH
#define LUALEARNSCRIPT_HH

#include "script/LuaScript.hpp"

#include <cstdint>

#include "types.hpp"

class Character;

class LuaLearnScript : public LuaScript {
	public:
		LuaLearnScript(const std::string& filename) { }
		LuaLearnScript(const LuaLearnScript&) = delete;
		LuaLearnScript& operator=(const LuaLearnScript&) = delete;

		void learn(Character *cc, TYPE_OF_SKILL_ID skill, uint32_t actionPoints, uint8_t opponent) { }
		void reduceMC(Character *cc) { }
};

#endif
