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

#ifndef __LUA_BINDING_SCRIPTITEM
#define __LUA_BINDING_SCRIPTITEM

class ScriptItem;
struct lua_State;

class ScriptItemWrapper {
	public:
		static void push(struct lua_State* state, const ScriptItem& item);
		static void Register(struct lua_State* state);
		static ScriptItem get(struct lua_State* state, int index);

	private:
		static void push_values(struct lua_State* state, const ScriptItem& item);
		static void get_values(struct lua_State* state, ScriptItem& item);
};

#endif
