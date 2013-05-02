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

#ifndef __LUA_BINDING_POSITION
#define __LUA_BINDING_POSITION

#include "Binder.hpp"

struct position;
struct lua_State;

class PositionWrapper : public Binder<position, CopyBinder<position>> {
	public:
		PositionWrapper();

		static int new_pos(lua_State* state);
		static int gc_fun(lua_State* state);
		static int to_string(lua_State* state);
		static int equal(lua_State* state);
		static int get_x(lua_State* state);
		static int set_x(lua_State* state);
		static int get_y(lua_State* state);
		static int set_y(lua_State* state);
		static int get_z(lua_State* state);
		static int set_z(lua_State* state);

		static PositionWrapper* instance() {
			if (_instance == nullptr)
				_instance = new PositionWrapper();
			return dynamic_cast<PositionWrapper*>(_instance);
		}

	protected:
		virtual void setup_functions() override;

};

#endif
