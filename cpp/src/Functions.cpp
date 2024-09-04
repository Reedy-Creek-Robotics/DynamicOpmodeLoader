#include "JFunc.hpp"
#include "LoadFunc.hpp"
#include "Lua.hpp"
#include "macros/Helpers.hpp"

#include <cstring>
#include <lua/lua.hpp>
#include <string>
#include <vector>

static lua_State* l;

std::vector<jobject> objects;

bool inClass = false;

void startClass()
{
	inClass = true;
	lua_newtable(l);
}

void endClass(const char* name)
{
	inClass = false;
	lua_setglobal(l, name);
}

std::vector<JFunc2> funcs;

int callJFunc(lua_State* l)
{
	int argc = lua_gettop(l) - 1;
	lua_getfield(l, 1, "id");

	int i = lua_tointeger(l, -1);

	JFunc2& fun = funcs[i];

	if (fun.argc != argc)
	{
		luaL_error(l, ("expected " + std::to_string(fun.argc) + " args, got " + std::to_string(argc)).c_str());
	}

	jvalue* args = nullptr;
	if (argc > 0)
	{
		args = new jvalue[argc];

		for (int i = 0; i < argc; i++)
		{
			int type = lua_type(l, i + 2);
			if (type != fun.argTypes[i])
			{
				const char* msg;
				const char* typearg; /* name for the type of the actual argument */
				if (luaL_getmetafield(l, i + 2, "__name") == LUA_TSTRING)
					typearg = lua_tostring(l, -1); /* use the given type name */
				else if (lua_type(l, i + 2) == LUA_TLIGHTUSERDATA)
					typearg = "light userdata"; /* special name for messages */
				else
					typearg = luaL_typename(l, i + 2); /* standard name */
				msg = lua_pushfstring(l, "%s expected, got %s", luaL_typename(l, fun.argTypes[i]), typearg);
				luaL_argerror(l, i + 1, msg);
        return 0;
			}
			switch (type)
			{
			case LUA_TNUMBER:
				args[i].d = lua_tonumber(l, i + 2);
				break;
			case LUA_TBOOLEAN:
				args[i].z = lua_toboolean(l, i + 2);
				break;
			case LUA_TSTRING: {
				const char* str = lua_tostring(l, i + 2);
				args[i].l = FuncStat::env->NewStringUTF(str);
				break;
			}
			default:
				break;
			}
		}
	}

	switch (fun.rtnType)
	{
	case LUA_TNONE: {
		bool rtn = fun.callB(args);
		if (rtn)
		{
			stop();
			luaL_error(l, "opmode stopped :)");
		}
		delete[] args;
		return 0;
	}
	case LUA_TNIL: {
		fun.callV(args);
		delete[] args;
		return 0;
	}
	case LUA_TNUMBER: {
		float rtn = fun.callF(args);
		lua_pushnumber(l, rtn);
		delete[] args;
		return 1;
	}
	case LUA_TBOOLEAN: {
		bool rtn = fun.callB(args);
		lua_pushboolean(l, rtn);
		delete[] args;
		return 1;
	}
	case LUA_TSTRING:
		jstring rtn = (jstring)fun.call(args);
		const char* str = FuncStat::env->GetStringUTFChars(rtn, nullptr);
		lua_pushstring(l, str);
		FuncStat::env->ReleaseStringUTFChars(rtn, str);
		delete[] args;
		return 1;
	}
	return 0;
}

void loadFuncs(lua_State* L)
{
	l = L;
	luaL_newmetatable(l, "jfunc::call");
	lua_pushcfunction(l, callJFunc);
	lua_setfield(l, -2, "__call");
}

void addObject(jobject object)
{
	jobject obj = FuncStat::env->NewGlobalRef(object);
	objects.push_back(obj);
	FuncStat::setVals(FuncStat::env, obj);
}

void addFunction(const char* name, const char* signature, int rtn, int argc)
{
	lua_newtable(l);
	lua_pushinteger(l, funcs.size());
	lua_setfield(l, -2, "id");
	luaL_getmetatable(l, "jfunc::call");
	lua_setmetatable(l, -2);

	if (inClass)
		lua_setfield(l, -2, name);
	else
		lua_setglobal(l, name);

	funcs.push_back({});

	JFunc2& fun = funcs[funcs.size() - 1];

	int len = strlen(signature);

	for (int i = 0; i < len; i++)
	{
		char c = signature[i];
		switch (c)
		{
		case 'Z':
			fun.argTypes.push_back(LUA_TBOOLEAN);
			break;
		case 'L':
			fun.argTypes.push_back(LUA_TSTRING);
			break;
		case 'D':
			fun.argTypes.push_back(LUA_TNUMBER);
			break;
		default:
			break;
		}
	}

	fun.init(name, signature, rtn, argc);
	JNIEnv* env = FuncStat::env;
	if (env->ExceptionCheck())
	{
		env->ExceptionDescribe();
		env->ExceptionClear();
		jniErr(
			("could not find function \'" + std::string(name) + "\' with signature \'" + std::string(signature) + '\'')
				.c_str());
	}
}

void deleteRefs()
{
	for (jobject& obj : objects)
	{
		FuncStat::env->DeleteGlobalRef(obj);
	}
	objects.clear();
	funcs.clear();
}
