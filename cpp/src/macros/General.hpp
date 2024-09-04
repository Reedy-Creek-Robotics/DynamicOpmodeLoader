#ifdef NewClass
#undef NewClass
#undef EndClass
#undef SetJavaObject
#undef err
#endif

#ifndef MacroDef

#define err()                                                                                                          \
	int* i;                                                                                                            \
	*i = 1

#define SetJavaObject(className)                                                                                       \
	FuncStat::clazz = FuncStat::env->FindClass(#className);                                                            \
	FuncStat::obj = objects[#className]

#define NewClass()                                                                                                     \
	lua_newtable(l);                                                                                                   \
	inClass = true

#define EndClass(name)                                                                                                 \
	lua_setglobal(l, #name);                                                                                           \
	inClass = false



#else

#define err()

#define SetJavaObject(className)

#define NewClass()

#define EndClass(name)

#endif
