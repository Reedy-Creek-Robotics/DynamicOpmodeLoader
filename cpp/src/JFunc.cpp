#include "JFunc.hpp"
#include "jni.h"

JNIEnv* FuncStat::env = nullptr;
jobject FuncStat::obj = {};
jclass FuncStat::clazz = {};
std::string FuncStat::storageDir = "";

void FuncStat::setVals(JNIEnv* _env, jobject& _obj)
{
	obj = _obj;
	env = _env;
	clazz = _env->GetObjectClass(_obj);
}
