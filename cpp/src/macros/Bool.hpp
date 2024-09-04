#ifdef FunctionB_V
#undef FunctionB_V
#endif

#ifndef MacroDef

#define FunctionB_V(name, funcName)                                                                                    \
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

#else

#define FunctionB_V(name, _)                                                                                           \
	JFunc<jboolean> name;                                                                                              \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		bool a = name.callB();                                                                                         \
		errCheck();                                                                                                    \
		lua_pushboolean(L, a);                                                                                         \
		return 1;                                                                                                      \
	}

#endif
