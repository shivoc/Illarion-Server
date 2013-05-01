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
