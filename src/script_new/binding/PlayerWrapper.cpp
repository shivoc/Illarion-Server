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

#include "script/binding/PlayerWrapper.hpp"

#include <lua.hpp>

#include "Player.hpp"
#include "World.hpp"

#include "script/LuaScript.hpp"

#include "script/binding/helper.hpp"

Player* PlayerIdBinder::getById(uint64_t id) {
	return dynamic_cast<Player*>(World::get()->findCharacter(id));
}

uint64_t PlayerIdBinder::getId(Player const* character) {
	return character->getId();
}

PlayerWrapper* PlayerWrapper::_instance = nullptr;

PlayerWrapper::PlayerWrapper() : Binder("Player") {
}

void PlayerWrapper::setup_functions() {
	_functions.static_data["english"] = ValueBinder<static_cast<int>(Language::english)>;
	_functions.static_data["german"] = ValueBinder<static_cast<int>(Language::german)>;
}

