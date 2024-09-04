#include "JFunc.hpp"
#include "LoadFunc.hpp"
#include <lua/lua.hpp>
#include <string>
#include <unordered_map>
#include <cstring>

static lua_State* l = nullptr;

bool running = true;

void stop()
{
  running = false;
}

JFunc<void, jstring> printF;
JFunc<void, jstring> errorF;
JFunc<void, jstring> jniErrorF;

std::unordered_map<std::string, int> opmodes;
int dispMarkerInd = 0;

void print(const char* str)
{
	jstring j = FuncStat::env->NewStringUTF(str);
	printF.callV(j);
	FuncStat::env->ReleaseStringUTFChars(j, FuncStat::env->GetStringUTFChars(j, nullptr));
}

void err(const char* str)
{
	jstring j = FuncStat::env->NewStringUTF(str);
	errorF.callV(j);
}

void jniErr(std::string msg)
{
	jstring j = FuncStat::env->NewStringUTF(msg.c_str());
	jniErrorF.callV(j);
}

std::string getPathName(const std::string& name)
{
	int i = -1;
	for (auto& [k, v] : opmodes)
	{
		if (name == k)
		{
			i = v;
			break;
		}
	}
	if (i == -1)
		err(("opmodes table doesnt contain opmode " + name).c_str());
	lua_getglobal(l, "Opmodes");
	lua_rawgeti(l, -1, i);
	lua_getfield(l, -1, "path");
	if (lua_type(l, -1) == LUA_TSTRING)
	{
		std::string str = lua_tostring(l, -1);
		lua_pop(l, 3);
		return str;
	}
	lua_pop(l, 3);
	return "";
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoader_internalInit(JNIEnv* env, jobject thiz, jobject stdlib)
{
	FuncStat::env = env;

	deleteRefs();

	if (l != nullptr)
	{
		lua_close(l);
	}

	addObject(stdlib);

	printF.init("print", "(Ljava/lang/String;)V");
	errorF.init("err", "(Ljava/lang/String;)V");
	jniErrorF.init("jniErr", "(Ljava/lang/String;)V");

	JFunc<jstring> getDataDir("getDataDir", "()Ljava/lang/String;");

	jstring dataDirJ = getDataDir.call();
	const char* dataDirC = env->GetStringUTFChars(dataDirJ, nullptr);
	std::string dataDir = dataDirC;
	env->ReleaseStringUTFChars(dataDirJ, dataDirC);

	FuncStat::storageDir = dataDir;

	l = luaL_newstate();
	luaL_openlibs(l);

	luaL_dostring(l, ("package.path = package.path .. ';" + dataDir + "/lua/?.lua'").c_str());

	loadFuncs(l);

	if (luaL_dofile(l, (dataDir + "/lua/main.lua").c_str()))
	{
		err(lua_tostring(l, -1));
		return nullptr;
	}
	lua_getglobal(l, "Opmodes");
	if (lua_type(l, -1) != LUA_TTABLE)
	{
		err("opmodes table must be a table");
		return nullptr;
	}
	lua_pushnil(l);

	int count = 0;
	while (lua_next(l, -2))
	{
		count++;
		lua_pushvalue(l, -2);
		std::string key = lua_tostring(l, -1);

		if (lua_type(l, -2) != LUA_TTABLE)
		{
			lua_pop(l, 2);
			continue;
		}

		lua_getfield(l, -2, "name");
		if (lua_type(l, -1) != LUA_TSTRING)
		{
			err(("name of opmode '" + key + "' must be a string").c_str());
			return nullptr;
		}
		std::string name = lua_tostring(l, -1);
		lua_pop(l, 1);
		opmodes.emplace(name, stoi(key));

		lua_pop(l, 2);
	}
	lua_pop(l, 1);

	jobjectArray arr = env->NewObjectArray(count, env->FindClass("java/lang/String"), env->NewStringUTF("T"));
	int i = 0;
	for (auto& [k, v] : opmodes)
	{
		env->SetObjectArrayElement(arr, i++, env->NewStringUTF(k.c_str()));
	}
	return arr;
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoader_start(JNIEnv* env, jobject thiz,
																							jstring name,
																							int recognition)
{
	lua_settop(l, 0);
	FuncStat::obj = thiz;
	lua_getglobal(l, "Opmodes");
	int ind = -1;
	const char* c = env->GetStringUTFChars(name, nullptr);
	for (auto& [k, v] : opmodes)
	{
		if (k == c)
		{
			ind = v;
			break;
		}
	}
	if (ind == -1)
	{
		err(("opmodes table doesnt contain opmode " + std::string(c)).c_str());
		return;
	}

	lua_rawgeti(l, -1, ind);
	lua_getfield(l, -1, "start");
	if (lua_type(l, -1) == LUA_TFUNCTION)
	{
		lua_pushinteger(l, recognition);
		if (lua_pcall(l, 1, 0, 0))
		{
			err(lua_tostring(l, -1));
			return;
		}
	}
	lua_settop(l, 1);
	lua_rawgeti(l, -1, ind);
	lua_getfield(l, -1, "markers");
	if (lua_type(l, -1) != LUA_TTABLE)
	{
		lua_pop(l, 1);
		lua_newtable(l);
	}
	dispMarkerInd = 0;
}

extern "C" JNIEXPORT jboolean JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoader_update(JNIEnv* env, jobject thiz,
																							 double deltaTime,
																							 double elapsedTime)
{
	lua_getfield(l, 2, "update");
	if (lua_isfunction(l, -1))
	{
		lua_newtable(l);
		lua_pushnumber(l, deltaTime);
		lua_setfield(l, -2, "delta");
		lua_pushnumber(l, elapsedTime);
		lua_setfield(l, -2, "elapsed");
		if (lua_pcall(l, 1, 0, 0))
		{
      if(running)
        err(lua_tostring(l, -1));
			return true;
		}
	}
  return false;
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoader_stop(JNIEnv* env, jobject thiz)
{
	lua_close(l);
	l = nullptr;
}

void callNextDispMarker(std::string str)
{
	if (str == "")
	{
		dispMarkerInd++;
		lua_rawgeti(l, 3, dispMarkerInd);
		if (lua_type(l, -1) == LUA_TFUNCTION)
		{
			if (lua_pcall(l, 0, 0, 0))
			{
				err(lua_tostring(l, -1));
				return;
			}
		}
	}
	else
	{
		lua_getfield(l, 3, str.c_str());
		if (lua_type(l, -1) == LUA_TFUNCTION)
		{
			if (lua_pcall(l, 0, 0, 0))
			{
				err(lua_tostring(l, -1));
				return;
			}
		}
	}
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_LuaFunctionBuilder_setCurrentObject(
	JNIEnv* env, jobject thiz, jobject thing)
{
	FuncStat::env = env;
	addObject(thing);
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_LuaFunctionBuilder_addFunction(
	JNIEnv* env, jobject thiz, jstring name, jstring signature, jint argc, jint rtnType)
{
	FuncStat::env = env;
	const char* name2 = env->GetStringUTFChars(name, nullptr);
	const char* signature2 = env->GetStringUTFChars(signature, nullptr);

	addFunction(name2, signature2, argc, rtnType);

	env->ReleaseStringUTFChars(name, name2);
	env->ReleaseStringUTFChars(signature, signature2);
}

extern "C" JNIEXPORT void JNICALL
Java_org_firstinspires_ftc_teamcode_opmodeloader_LuaFunctionBuilder_newClass(JNIEnv* env, jobject thiz)
{
	startClass();
}

extern "C" JNIEXPORT void JNICALL
Java_org_firstinspires_ftc_teamcode_opmodeloader_LuaFunctionBuilder_endClass(JNIEnv* env, jobject thiz, jstring name)
{
	const char* str = env->GetStringUTFChars(name, nullptr);
	endClass(str);
	env->ReleaseStringUTFChars(name, str);
}
