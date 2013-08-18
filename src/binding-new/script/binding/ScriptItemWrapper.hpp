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

#include "Binder.hpp"
#include "ItemWrapper.hpp"

class ScriptItem;
struct lua_State;

class ScriptItemWrapper : public Binder<ScriptItem, CopyBinder<ScriptItem>, ItemWrapper> {
	public:
		ScriptItemWrapper();

		static ScriptItemWrapper* instance() {
			if (_instance == nullptr)
				_instance = new ScriptItemWrapper();
			return dynamic_cast<ScriptItemWrapper*>(_instance);
		}

		static int create(lua_State* state);
		static int gc_fun(lua_State* state);
		static int getOwner(lua_State* state);
		static int getPosition(lua_State* state);
		static int getItemposition(lua_State* state);
		static int getInside(lua_State* state);
		static int getType(lua_State* state);


	protected:
		virtual void setup_functions() override;
};

#endif
