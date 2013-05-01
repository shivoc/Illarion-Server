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

#ifndef __LUA_BINDING_PLAYER
#define __LUA_BINDING_PLAYER

#include "Binder.hpp"
#include "CharacterWrapper.hpp"

class Player;
struct lua_State;

class PlayerIdBinder : public IdBinder<Player> {
	protected:
		virtual Player* getById(uint64_t id) override;
		virtual uint64_t getId(Player const* character) override;
};

class PlayerWrapper : public Binder<Player, PlayerIdBinder, CharacterWrapper> {
	public:
		PlayerWrapper();
		static int inform(struct lua_State* state);

		static PlayerWrapper* instance() {
			if (_instance == nullptr)
				_instance = new PlayerWrapper();
			return _instance;
		}

	protected:
		static PlayerWrapper* _instance;
		virtual void setup_functions() override;
};

#endif
