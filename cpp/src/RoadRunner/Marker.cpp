#include "JFunc.hpp"
#include "Lua.hpp"
#include "Save.hpp"
#include <jni.h>
#include <string>

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_TrajectoryBuilder_callDisplacement(
	JNIEnv* env, jobject thiz, jstring str)
{
	callNextDispMarker(env->GetStringUTFChars(str, nullptr));
}

NodeGrid grid;

void setup();

jobject builder2;

extern "C" JNIEXPORT void JNICALL
Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoaderRR_internalInit(JNIEnv* env, jobject thiz, jobject builder)
{
	builder2 = env->NewGlobalRef(builder);
	FuncStat::setVals(env, builder2);

	Save::makeBuilder.init("makeBuilder", "(DDD)V");
	Save::lineTo.init("lineTo", "(DD)V");
	Save::splineTo.init("splineTo", "(DDD)V");
	Save::lineToLinearHeading.init("lineToLinearHeading", "(DDD)V");
	Save::splineToLinearHeading.init("splineToLinearHeading", "(DDDD)V");
	Save::lineToConstantHeading.init("lineToConstantHeading", "(DD)V");
	Save::splineToConstantHeading.init("splineToConstantHeading", "(DDD)V");
	Save::lineToSplineHeading.init("lineToSplineHeading", "(DDD)V");
	Save::splineToSplineHeading.init("splineToSplineHeading", "(DDDD)V");
	Save::marker.init("marker", "(Ljava/lang/String;)V");
	Save::wait.init("wait", "(D)V");
	Save::rotate.init("turn", "(D)V");
	Save::pathErr.init("pathErr", "(Ljava/lang/String;)V");
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoaderRR_buildPath(
	JNIEnv* env, jobject thiz, jstring name, int recognition)
{
	std::string str = env->GetStringUTFChars(name, nullptr);
	std::string path = getPathName(str);
	if (path == "")
	{
		return;
	}
	bool rtn = Save::load(&grid, (FuncStat::storageDir + "/paths/" + path));
	if (rtn == false)
	{
		return;
	}
	grid.recognitionId = recognition;
	Save::exp(&grid);
}

extern "C" JNIEXPORT void JNICALL Java_org_firstinspires_ftc_teamcode_opmodeloader_OpmodeLoaderRR_close(JNIEnv* env,
																									  jobject thiz)
{
	env->DeleteGlobalRef(builder2);
}
