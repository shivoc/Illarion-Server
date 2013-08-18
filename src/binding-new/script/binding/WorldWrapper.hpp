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

#ifndef __LUA_BINDING_WORLD
#define __LUA_BINDING_WORLD

#include "Binder.hpp"

class World;
struct lua_State;

class WorldBinder {
	public:
		void push(lua_State* state, const World& target, const std::string& classname);
		World* get(lua_State* state, int index, const std::string& classname);
};

class WorldWrapper : public Binder<World, WorldBinder> {
	public:
		WorldWrapper();
		static int getItemName(struct lua_State* state);
		static int itemInform(struct lua_State* state);

		static WorldWrapper* instance() {
			if (_instance == nullptr)
				_instance = new WorldWrapper();
			return dynamic_cast<WorldWrapper*>(_instance);
		}

	protected:
		virtual void setup_functions() override;
};

#endif
