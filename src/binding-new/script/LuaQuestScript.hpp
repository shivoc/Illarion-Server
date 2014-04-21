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

#ifndef LUAQUESTSCRIPT_HH
#define LUAQUESTSCRIPT_HH

#include "types.hpp"
#include "script/LuaScript.hpp"

class Character;

class LuaQuestScript : public LuaScript {
	public:
		LuaQuestScript(const std::string& filename, TYPE_OF_QUEST_ID quest) { }
		LuaQuestScript(const LuaQuestScript&) = delete;
		LuaQuestScript& operator=(const LuaQuestScript&) = delete;

		std::string title(Character *user) { return {}; }
		std::string description(Character *user, TYPE_OF_QUESTSTATUS status) { return {}; }
		void targets(Character *user, TYPE_OF_QUESTSTATUS status, std::vector<position> &targets) { }
		TYPE_OF_QUESTSTATUS finalStatus() { return {}; }
		position start() { return {}; };
		QuestAvailability available(Character *user, TYPE_OF_QUESTSTATUS status) { return {}; }
};

#endif
