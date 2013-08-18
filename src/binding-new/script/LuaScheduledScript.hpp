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

#ifndef LUASCHEDULEDSCRIPT_HH
#define LUASCHEDULEDSCRIPT_HH

#include "script/LuaScript.hpp"

class LuaScheduledScript : public LuaScript {
	public:
		LuaScheduledScript(const std::string& filename) { }
		LuaScheduledScript(const LuaScheduledScript&) = delete;
		LuaScheduledScript& operator=(const LuaScheduledScript&) = delete;

		void callFunction(const std::string &name, uint32_t currentCycle, uint32_t lastCycle, uint32_t nextCycle) { }
};

#endif
