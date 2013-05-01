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

#ifndef __BINDER_H
#define __BINDER_H

#include "BindHelper.hpp"

class NoBase {
	public:
		static BindFunctions _functions;
};

template<typename Target>
class PointerBinder {
	public:
		void push(lua_State* state, const Target& target, const std::string& classname) {
			BindHelper::push_ptr_userdata(state, (void*)&target, sizeof(Target**), classname);
		}

		Target* get(lua_State* state, int index, const std::string& classname) {
			Target** raw = (Target**)BindHelper::get_ptr_userdata(state, index, classname);
			Target* ptr = dynamic_cast<Target*>(*raw);
			if (ptr == nullptr) {
				BindHelper::arg_error(state, index, classname);
				return nullptr;
			}
			return ptr;
		}
};

template<typename Target>
class IdBinder {
	public:
		void push(lua_State* state, const Target& target, const std::string& classname) {
			BindHelper::push_id(state, getId(&target), classname);
		}

		Target* get(lua_State* state, int index, const std::string& classname) {
			uint64_t id = BindHelper::get_id(state, index, classname);
			return getById(id);
			//return getById(BindHelper::get_id(state, index, classname));
		}

	protected:
		virtual Target* getById(uint64_t id) = 0;
		virtual uint64_t getId(Target const*) = 0;
};

template<typename Target, typename BindType = PointerBinder<Target>, typename Base = NoBase>
class Binder {
	public:
		static int resolve_index(lua_State* state) {
			return BindHelper::resolve_index(state, _functions);
		}

		static int new_index(lua_State* state) {
			return BindHelper::new_index(state, _functions);
		}

		static int resolve_static(lua_State* state) {
			return BindHelper::resolve_static(state, _functions);
		}

		void Register(lua_State* state) {
			setup_functions();
			BindHelper::Register(state, _classname, &resolve_index, &new_index, _functions.constructor, _functions.gc);
			if (!std::is_same<Base, NoBase>::value) {
				_functions.parent = &Base::_functions;
			}
			if (!_functions.static_data.empty()) {
				BindHelper::RegisterGlobal(state, _classname, &resolve_static);
			}
		}

		void push(lua_State* state, const Target& target) {
			BindType().push(state, target, _classname);
		}


		Target* get(lua_State* state, int index) {
			return BindType().get(state, index, _classname);
		}

		static BindFunctions _functions;

		virtual ~Binder() { }
	protected:
		virtual void setup_functions() = 0;
		const std::string _classname;
		Binder(const std::string& classname) : _classname{classname} {
			BindHelper::add_deregister_function([] {
				delete Binder<Target, BindType, Base>::_instance;
				Binder<Target, BindType, Base>::_instance = nullptr;
			});
		}
		static Binder<Target, BindType, Base>* _instance;
};

template<typename foo, typename bar, typename baz>
BindFunctions Binder<foo, bar, baz>::_functions;

template<typename foo, typename bar, typename baz>
Binder<foo, bar, baz>* Binder<foo, bar, baz>::_instance = nullptr;

template<uint64_t value>
int ValueBinder(lua_State* state) {
	return BindHelper::push_int(state, value);
}

#endif
