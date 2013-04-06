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

#ifndef LUASCRIPT_HH
#define LUASCRIPT_HH

#include <stdexcept>
#include <memory>
#include <string>

#include "globals.hpp"
#include "Item.hpp"

class Character;

enum SouTarTypes {
    LUA_NONE = 0, /**< not a correct type (only for initialisation) */
    LUA_FIELD = 1,  /**< target was a field */
    LUA_ITEM = 2, /**< target was a item */
    LUA_CHARACTER = 3, /**< target was character*/
    LUA_DIALOG = 4
};

enum LtaStates {
	LTS_NOLTACTION = 0,        /**< no longtime action in this script */
	LTS_ACTIONABORTED = 1,     /**< long time action was aborted */
	LTS_ACTIONSUCCESSFULL = 2  /**< long time action was performed sucessfulle*/
};

struct SouTar {
	Character *character = nullptr;  /**< Source or target is a character, this is the pointer to it, otherwise NULL */
	ScriptItem item;                 /**< Source or target is a Item, this holds the information of the item */
	SouTarTypes Type = LUA_NONE;     /**< Source or Target Type (if its an character, field or item) */
	position pos;                    /**< aboslute position of the object */
	unsigned int dialog = 0;
};

class ScriptException : public std::runtime_error {
	public:
		ScriptException(const std::string& message) : std::runtime_error(message) { }
};

struct lua_State;

class LuaScript {
	public:
		LuaScript() { }
		LuaScript(const std::string& filename) { };
		LuaScript(const std::string &code, const std::string &codename);
		LuaScript(const LuaScript &) = delete;
		LuaScript &operator=(const LuaScript &) = delete;;

		static void triggerScriptError(const std::string &msg) { };
		std::string getFileName() const {  return {}; }
		void addQuestScript(const std::string &entrypoint, const std::shared_ptr<LuaScript> &script) { }

		static void shutdownLua() { }
		bool existsEntrypoint(const std::string &entrypoint) { return false; }

		virtual ~LuaScript() { }

		static void writeDebugMsg(const std::string &msg) { }

		template<typename T>
		static void executeDialogCallback(T &dialog) { }

		template<typename U, typename T>
		static U executeDialogCallback(T &dialog) { return {}; }

	protected:
		static struct lua_State* _luaState;
		void logCurrentError();
		int add_backtrace(struct lua_State*);

	private:
		static void init_state();
};

#endif
