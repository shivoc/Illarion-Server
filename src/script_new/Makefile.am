AM_CXXFLAGS = -ggdb -pipe -Wall -Werror -Wno-deprecated -std=c++11 $(BOOST_CXXFLAGS) $(DEPS_CFLAGS)
AM_CPPFLAGS = -D_THREAD_SAFE -D_REENTRANT -DTESTSERVER -DCDataConnect_DEBUG -DAdminCommands_DEBUG $(BOOST_CPPFLAGS) -I$(top_srcdir)/src

noinst_LTLIBRARIES = libscriptbinding.la

libscriptbinding_la_SOURCES = LuaScript.cpp LuaItemScript.cpp \
			      LuaItemScript.cpp \
			      binding/helper.cpp \
			      binding/CharacterWrapper.cpp binding/WorldWrapper.cpp \
			      binding/PlayerWrapper.cpp binding/ItemLookAtWrapper.cpp \
			      binding/ScriptItemWrapper.cpp binding/ItemWrapper.cpp \
			      binding/ContainerWrapper.cpp

noinst_HEADERS = LuaScript.hpp
