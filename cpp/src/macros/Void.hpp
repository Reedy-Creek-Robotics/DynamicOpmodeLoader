#ifdef FunctionV_V
#undef FunctionV_V
#undef FunctionV_D
#undef FunctionV_B
#undef FunctionV_S
#undef FunctionV_SS
#endif

#ifndef MacroDef

#define FunctionV_D(name, funcName)                                                                                    \
	name.init(#name, "(D)V");                                                                                          \
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

#define FunctionV_S(name, funcName)                                                                                    \
	name.init(#name, "(Ljava/lang/String;)V");                                                                         \
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

#define FunctionV_SS(name, funcName)                                                                                   \
	name.init(#name, "(Ljava/lang/String;Ljava/lang/String;)V");                                                       \
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

#define FunctionV_V(name, funcName)                                                                                    \
	name.init(#name, "()V");                                                                                           \
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

#define FunctionV_B(name, funcName)                                                                                    \
	name.init(#name, "(Z)V");                                                                                          \
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

#define FunctionV_D(name, _)                                                                                           \
	JFunc<void, jdouble> name;                                                                                         \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		TypeCheck(1, LUA_TNUMBER);                                                                                     \
		jdouble i = lua_tonumber(L, -1);                                                                               \
		name.callV(i);                                                                                                 \
		errCheck();                                                                                                    \
		return 0;                                                                                                      \
	}

#define FunctionV_S(name, _)                                                                                           \
	JFunc<void, jstring> name;                                                                                         \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		TypeCheck(1, LUA_TSTRING);                                                                                     \
		const char* i = lua_tostring(L, -1);                                                                           \
		jstring j = FuncStat::env->NewStringUTF(i);                                                                    \
		name.callV(j);                                                                                                 \
		errCheck();                                                                                                    \
		FuncStat::env->ReleaseStringUTFChars(j, FuncStat::env->GetStringUTFChars(j, NULL));                            \
		return 0;                                                                                                      \
	}

#define FunctionV_SS(name, _)                                                                                          \
	JFunc<void, jstring, jstring> name;                                                                                \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		TypeCheck(1, LUA_TSTRING);                                                                                     \
		TypeCheck(2, LUA_TSTRING);                                                                                     \
		const char* i1 = lua_tostring(L, 1);                                                                           \
		const char* i2 = lua_tostring(L, 2);                                                                           \
		jstring j1 = FuncStat::env->NewStringUTF(i1);                                                                  \
		jstring j2 = FuncStat::env->NewStringUTF(i2);                                                                  \
		name.callV(j1, j2);                                                                                            \
		errCheck();                                                                                                    \
		FuncStat::env->ReleaseStringUTFChars(j1, FuncStat::env->GetStringUTFChars(j1, NULL));                          \
		FuncStat::env->ReleaseStringUTFChars(j2, FuncStat::env->GetStringUTFChars(j2, NULL));                          \
		return 0;                                                                                                      \
	}

#define FunctionV_V(name, _)                                                                                           \
	JFunc<void> name;                                                                                                  \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		name.callV();                                                                                                  \
		errCheck();                                                                                                    \
		return 0;                                                                                                      \
	}

#define FunctionV_B(name, _)                                                                                           \
	JFunc<void, jboolean> name;                                                                                        \
	int name##F(lua_State* L)                                                                                          \
	{                                                                                                                  \
		TypeCheck(1, LUA_TBOOLEAN);                                                                                    \
		bool a = lua_toboolean(L, -1);                                                                                 \
		name.callV(a);                                                                                                 \
		errCheck();                                                                                                    \
		return 0;                                                                                                      \
	}

#endif
