#pragma once
#include <jni.h>
#include <string>
#include <vector>

struct FuncStat
{
	static jobject obj;
	static jclass clazz;
	static JNIEnv* env;
	static std::string storageDir;
	static void setVals(JNIEnv* _env, jobject& _obj);
};

template <typename T, typename... Args> class JFunc
{
  public:
	JFunc()
	{
	}
	JFunc(const char* name, const char* sig)
	{
		init(name, sig);
	}
	void init(const char* name, const char* sig)
	{
		method = FuncStat::env->GetMethodID(FuncStat::clazz, name, sig);
		obj = FuncStat::obj;
	}
	void callV(Args... args)
	{
		FuncStat::env->CallVoidMethod(obj, method, args...);
	}
	bool callB(Args... args)
	{
		return FuncStat::env->CallBooleanMethod(obj, method, args...);
	}
	T call(Args... args)
	{
		return (T)FuncStat::env->CallObjectMethod(obj, method, args...);
	}

  private:
	jmethodID method;
	jobject obj;
};

class JFunc2
{
  public:
	char argc;
	int rtnType;
	std::vector<char> argTypes;
	JFunc2()
	{
	}
	JFunc2(const char* name, const char* sig, int rtnType, char argc)
	{
		init(name, sig, rtnType, argc);
	}
	void init(const char* name, const char* sig, int rtnType2, char argc2)
	{
		method = FuncStat::env->GetMethodID(FuncStat::env->GetObjectClass(FuncStat::obj), name, sig);
		obj = FuncStat::obj;
		argc = argc2;
		rtnType = rtnType2;
	}
	void callV(jvalue* args)
	{
		FuncStat::env->CallVoidMethodA(obj, method, args);
	}
	bool callB(jvalue* args)
	{
		return FuncStat::env->CallBooleanMethodA(obj, method, args);
	}
	bool callF(jvalue* args)
	{
		return FuncStat::env->CallFloatMethodA(obj, method, args);
	}
	jobject call(jvalue* args)
	{
		return FuncStat::env->CallObjectMethod(obj, method, args);
	}

  private:
	jmethodID method;
	jobject obj;
};
