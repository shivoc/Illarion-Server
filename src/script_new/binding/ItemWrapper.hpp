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

#ifndef __LUA_BINDING_ITEM
#define __LUA_BINDING_ITEM

#include "Binder.hpp"

class Item;
struct lua_State;

// TODO move to IdBinder
class ItemWrapper : public Binder<Item> {
	public:
		ItemWrapper();

		static int getId(lua_State* state);
		static int setId(lua_State* state);

		static ItemWrapper* instance() {
			if (_instance == nullptr)
				_instance = new ItemWrapper();
			return _instance;
		}

	protected:
		static ItemWrapper* _instance;
		virtual void setup_functions() override;

};

#endif
