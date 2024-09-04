#define TypeCheck(ind, type)                                                                                           \
	if (lua_type(L, ind) != type)                                                                                      \
		luaL_error(L, "arg " #ind "must be " #type)
