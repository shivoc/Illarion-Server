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

#include "script/binding/ScriptItemWrapper.hpp"

#include <lua.hpp>

#include "Item.hpp"

#include "script/binding/helper.hpp"
#include "script/binding/ItemWrapper.hpp"

ScriptItemWrapper* ScriptItemWrapper::_instance = nullptr;

ScriptItemWrapper::ScriptItemWrapper() : Binder("ScriptItem") {
}

void ScriptItemWrapper::setup_functions() {
}

/*
        luabind::class_<ScriptItem,Item>("scriptItem")
        .def(luabind::constructor<>())
        .def_readonly("owner", &ScriptItem::getOwnerForLua)
        .def_readonly("pos", &ScriptItem::pos)
        .def("getType", &ScriptItem::getType)
        .def_readonly("itempos", &ScriptItem::itempos)
        .def_readonly("inside", &ScriptItem::inside)
*/

#if 0
void ScriptItemWrapper::push(struct lua_State* state, const ScriptItem& item) {
	lua_newtable(state);
	luaL_getmetatable(state, CLASS_TABLE_NAME);
	lua_setmetatable(state, -2);

	push_values(state, item);
}

void ScriptItemWrapper::push_values(struct lua_State* state, const ScriptItem& item) {
	ItemWrapper::push_values(state, item);
}

void ScriptItemWrapper::get_values(struct lua_State* state, ScriptItem& item) {
}

void ScriptItemWrapper::Register(struct lua_State* state) {
	luaH_register_struct(state, CLASS_TABLE_NAME, CLASS_BASE_TABLE_NAME);
}

ScriptItem ScriptItemWrapper::get(struct lua_State* state, int index) {
	luaL_checktype(state, index, LUA_TTABLE);
	lua_getmetatable(state, index);
	luaL_getmetatable(state, CLASS_TABLE_NAME);
	int equal = lua_rawequal(state, -1, -2);
	lua_pop(state, 2);
		
	if (!equal) {
		luaL_typerror(state, index, CLASS_TABLE_NAME);
	}

	// TODO copy item to top of stack
	ScriptItem item;

	get_values(state, item);
	// TODO pop copy again

	return item;
}
#endif
