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

#ifndef __LUA_BINDING_ITEMLOOKAT
#define __LUA_BINDING_ITEMLOOKAT

#include "Binder.hpp"

class ItemLookAt;
struct lua_State;

class ItemLookAtWrapper : public Binder<ItemLookAt> {
	public:
		ItemLookAtWrapper();

		//static void Register(struct lua_State* state);
		//static ItemLookAt get(struct lua_State* state, int index);

		static int create(struct lua_State* state);

		//static void fillSetFunctions(struct lua_State* state);
		//static void fillGetFunctions(struct lua_State* state);

		static int getName(struct lua_State* state);
		static int setName(struct lua_State* state);
		static int gc_fun(struct lua_State* state);

		static ItemLookAtWrapper* instance() {
			if (_instance == nullptr)
				_instance = new ItemLookAtWrapper();
			return dynamic_cast<ItemLookAtWrapper*>(_instance);
		}

	protected:
		virtual void setup_functions() override;
};

#endif
