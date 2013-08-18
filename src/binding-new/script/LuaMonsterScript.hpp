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

#ifndef LUAMONSTERSCRIPT_HH
#define LUAMONSTERSCRIPT_HH

#include "script/LuaScript.hpp"

#include "Character.hpp"

class Player;

class LuaMonsterScript : public LuaScript {
	public:
		LuaMonsterScript() { };
		LuaMonsterScript(const std::string& filename) { };
		LuaMonsterScript(const LuaMonsterScript &) = delete;
		LuaMonsterScript &operator=(const LuaMonsterScript &) = delete;;

		void receiveText(Character *Monster, Character::talk_type tt, const std::string &message, Character *cc) { }
		void onDeath(Character *Monster) { }
		void onAttacked(Character *Monster, Character *attacker) { }
		void onCasted(Character *Monster, Character *caster) { }
		void useMonster(Character *Monster, Character *user, unsigned char ltastate) { }
		void lookAtMonster(Character *source, Character *target, unsigned char mode) { }
		bool actionDisturbed(Character *performer, Character *disturber) { return false; }
		void onAttack(Character *Monster, Character *target) { }
		bool enemyOnSight(Character *Monster, Character *enemy) { return false; }
		bool enemyNear(Character *Monster, Character *enemy) { return false; }
		void abortRoute(Character *Monster) { }
		void onSpawn(Character *Monster) { }
		bool setTarget(Character *Monster, const std::vector<Player*> &CandidateList, Player *&Target) { return false; }
};

#endif
