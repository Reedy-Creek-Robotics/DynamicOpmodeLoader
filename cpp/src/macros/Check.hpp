#ifdef FunctionC_D
#undef FunctionC_D
#endif

#ifndef MacroDef

#define FunctionC_V(name, funcName)                                                                                    \
	name.init(#name, "()Z");                                                                                           \
	errCheck();                                                                                                        \
	lua_pushcfunction(l, name##F);                                                                                     \
	if (!inClass)                                                                                                      \
	{                                                                                                                  \
		lua_setglobal(l, #funcName);                                                                                   \
	}                                                                                                                  \
	else                                                                                                               \
	{                                                                                                                  \
		lua_setfield(l, -2, #funcName);                                                                                \
	}

#define FunctionC_D(name, funcName)                                                                                    \
	name.init(#name, "(D)Z");                                                                                          \
	errCheck();                                                                                                        \
	lua_pushcfunction(l, name##F);                                                                                     \
	if (!inClass)                                                                                                      \
	{                                                                                                                  \
		lua_setglobal(l, #funcName);                                                                                   \
	}                                                                                                                  \
	else                                                                                                               \
	{                                                                                                                  \
		lua_setfield(l, -2, #funcName);                                                                                \
	}

#else

#define FunctionC_V(name, _)                                                                                           \
	JFunc<jboolean> name;                                                                                              \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		bool a = name.callB();                                                                                         \
		errCheck();                                                                                                    \
		if (a)                                                                                                         \
		{                                                                                                              \
			luaL_error(L, "robot stopped :)");                                                                         \
		}                                                                                                              \
		return 0;                                                                                                      \
	}

#define FunctionC_D(name, _)                                                                                           \
	JFunc<jboolean, jdouble> name;                                                                                     \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		TypeCheck(1, LUA_TNUMBER);                                                                                     \
		jdouble i = lua_tonumber(L, -1);                                                                               \
		bool a = name.callB(i);                                                                                        \
		errCheck();                                                                                                    \
		if (a)                                                                                                         \
		{                                                                                                              \
			luaL_error(L, "robot stopped :)");                                                                         \
		}                                                                                                              \
		return 0;                                                                                                      \
	}

#endif
