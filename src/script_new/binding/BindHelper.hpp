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

#ifndef __BINDHELPER_H
#define __BINDHELPER_H

#include <map>
#include <string>
#include <vector>
#include <functional>

typedef int(*lua_function)(struct lua_State*);
typedef std::map<std::string, lua_function> function_map;

template<typename T>
void throw_ptr(void* ptr) {
	throw static_cast<T*>(ptr);
};

class type_caster {
	public:
		template<typename T>
			type_caster(T* ptr) {
				_ptr = ptr;
				_throw_func = throw_ptr<T>;// [](void* ptr) {
					//throw static_cast<T*>(ptr);
				//};
			}

			// null object
			explicit type_caster() {
				_ptr = nullptr;
				_throw_func = throw_ptr<void>;//[](void* ptr) {};
			}

		template<typename T>
			T* cast() {
				try {
					_throw_func(_ptr);
				} catch (T* ptr) {
					return ptr;
				}
				return nullptr;
			}

	private:
		std::function<void(void* ptr)> _throw_func;
		void* _ptr;
};

struct BindFunctions {
	function_map new_index;
	function_map mem_fun;
	function_map accessors;
	lua_function constructor = nullptr;
	function_map static_data;
	function_map static_methods;
	lua_function gc = nullptr;
	lua_function to_string = nullptr;
	lua_function eq = nullptr;

	BindFunctions* parent = nullptr;
};

class BindHelper {
	public:
		static int resolve_index(lua_State* state, const BindFunctions& functions);
		static int new_index(lua_State* state, const BindFunctions& functions);
		static int resolve_static(lua_State* state, const BindFunctions& functions);
		static void Register(lua_State* state, const std::string& classname, lua_function index, lua_function newindex, const BindFunctions& functions);
		static void RegisterGlobal(lua_State* state, const std::string& name, lua_function index);
		static void push_ptr_userdata(lua_State* state, type_caster caster, const std::string& classname);
		static type_caster get_ptr_userdata(lua_State* state, int index, const std::string& classname);
		static void arg_error(lua_State* state, int index, const std::string& classname);
		static void numarg_error(lua_State* state, const std::string& message);
		static void push_id(lua_State* state, uint64_t id, const std::string& classname);
		static uint64_t get_id(lua_State* state, int index, const std::string& classname);
		static int push_int(lua_State* state, uint64_t value);
		static void add_deregister_function(std::function<void()>);
		static void unregister();
	private:
		static std::vector<std::function<void()>> _deregister_functions;
};

#endif
