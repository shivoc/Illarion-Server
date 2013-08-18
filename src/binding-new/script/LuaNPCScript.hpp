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

#ifndef LUANPCSCRIPT_HH
#define LUANPCSCRIPT_HH

#include "script/LuaScript.hpp"

class NPC;
class Character;

class LuaNPCScript : public LuaScript {
	public:
		LuaNPCScript(const std::string& filename, NPC* npc) { };
		LuaNPCScript(const LuaNPCScript &) = delete;
		LuaNPCScript &operator=(const LuaNPCScript &) = delete;;

		void nextCycle() { }
		void receiveText(Character::talk_type tt, const std::string &message, Character *cc) { }
		void useNPC(Character *user, unsigned char ltastate) { }
		void lookAtNpc(Character *source, unsigned char mode) { }
		bool actionDisturbed(Character *performer, Character *disturber) { return false; }
		void abortRoute() { }
};

#endif
