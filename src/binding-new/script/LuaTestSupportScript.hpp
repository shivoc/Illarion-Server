/*
 *  illarionserver - server for the game Illarion
 *  Copyright 2011 Illarion e.V.
 *
 *  This file is part of illarionserver.
 *
 *  illarionserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  illarionserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LUA_TESTSUPPORT_SCRIPT_HPP_
#define _LUA_TESTSUPPORT_SCRIPT_HPP_

#include "LuaScript.hpp"
#include "Character.hpp"

class World;

template<typename T>
struct dependent_false: std::false_type {};

class LuaTestSupportScript : public LuaScript {
public:
    LuaTestSupportScript(const std::string &code, const std::string &scriptname = "test");
    virtual ~LuaTestSupportScript() noexcept;

    template<typename Ret, typename... Args>
	   Ret test(const Args &... args) {
	    static_assert(dependent_false<Ret>::value, "missing specialization");
	    return {};
	   }

private:

    LuaTestSupportScript(const LuaTestSupportScript &) = delete;
    LuaTestSupportScript &operator=(const LuaTestSupportScript &) = delete;
};

template<>
Item LuaTestSupportScript::test(const Item&);

template<>
ScriptItem LuaTestSupportScript::test(const ScriptItem&);

template<>
int LuaTestSupportScript::test(const int&);

template<>
position LuaTestSupportScript::test(const position&);

struct WeatherStruct;

template<>
WeatherStruct LuaTestSupportScript::test(const WeatherStruct&);

class Monster;
class Player;

template<>
bool LuaTestSupportScript::test(Monster* const&);

template<>
int LuaTestSupportScript::test(Monster* const&);

template<>
std::string LuaTestSupportScript::test(Monster* const&);

template<>
bool LuaTestSupportScript::test(Monster* const&, const ScriptItem&);

template<>
bool LuaTestSupportScript::test(Monster* const&, Player* const&);

template<>
direction LuaTestSupportScript::test(Monster* const&);

template<>
unsigned LuaTestSupportScript::test(Monster* const&);

template<>
short unsigned LuaTestSupportScript::test(Monster* const&);

template<>
short LuaTestSupportScript::test(Monster* const&);

template<>
Character::face_to LuaTestSupportScript::test(Monster* const&);

template<>
ScriptItem LuaTestSupportScript::test(Monster* const&);

#endif

