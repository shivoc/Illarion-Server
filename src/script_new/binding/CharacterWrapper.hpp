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

#ifndef __LUA_BINDING_CHARACTER
#define __LUA_BINDING_CHARACTER

#include "Binder.hpp"

class Character;
struct lua_State;

class CharIdBinder : public IdBinder<Character> {
	protected:
		virtual Character* getById(uint64_t id) override;
		virtual uint64_t getId(Character const* character) override;
};

class CharacterWrapper : public Binder<Character, CharIdBinder> {
	public:
		CharacterWrapper();
		static int inform(struct lua_State* state);
		static int getBackPack(struct lua_State* state);
		static CharacterWrapper* instance() {
			if (_instance == nullptr)
				_instance = new CharacterWrapper();
			return dynamic_cast<CharacterWrapper*>(_instance);
		}

	protected:
		virtual void setup_functions() override;
};

#endif
