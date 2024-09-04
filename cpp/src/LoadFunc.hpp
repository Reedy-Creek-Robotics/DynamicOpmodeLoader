#pragma once
#include <jni.h>
#include <lua/lua.hpp>
void endClass(const char* name);
void startClass();
void addFunction(const char* name, const char* signature, int rtn, int argc);
void loadFuncs(lua_State* l);
void addObject(jobject object);
void deleteRefs();
