if ENABLE_NEWSCRIPT
SCRIPT_BASE=binding-new
else
SCRIPT_BASE=binding-old
endif

AM_CXXFLAGS = -ggdb -pipe -Wall -Werror -Wno-deprecated -std=c++11 $(BOOST_CXXFLAGS) $(DEPS_CFLAGS)
AM_CPPFLAGS = -D_THREAD_SAFE -D_REENTRANT -DTESTSERVER -DCDataConnect_DEBUG -DAdminCommands_DEBUG $(subst -I,-isystem,$(BOOST_CPPFLAGS)) -I$(top_srcdir)/src -I$(top_srcdir)/src/$(SCRIPT_BASE)

noinst_LTLIBRARIES = libscriptbinding.la

libscriptbinding_la_SOURCES = LuaScript.cpp LuaItemScript.cpp \
			      LuaItemScript.cpp LuaTestSupportScript.cpp \
			      binding/helper.cpp binding/PositionWrapper.cpp \
			      binding/CharacterWrapper.cpp binding/WorldWrapper.cpp \
			      binding/PlayerWrapper.cpp binding/ItemLookAtWrapper.cpp \
			      binding/ScriptItemWrapper.cpp binding/ItemWrapper.cpp \
			      binding/ContainerWrapper.cpp binding/BindHelper.cpp

noinst_HEADERS = LuaDepotScript.hpp LuaItemScript.hpp LuaLearnScript.hpp \
		 LuaLoginScript.hpp LuaLogoutScript.hpp \
		 LuaLongTimeEffectScript.hpp LuaLookAtItemScript.hpp \
		 LuaLookAtPlayerScript.hpp LuaMagicScript.hpp \
		 LuaMonsterScript.hpp LuaNPCScript.hpp \
		 LuaPlayerDeathScript.hpp LuaQuestScript.hpp \
		 LuaReloadScript.hpp LuaScheduledScript.hpp LuaScript.hpp \
		 LuaTestSupportScript.hpp LuaTileScript.hpp \
		 LuaTriggerScript.hpp LuaWeaponScript.hpp
