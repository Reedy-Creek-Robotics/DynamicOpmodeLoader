package org.firstinspires.ftc.teamcode.opmodeloader

import android.os.Environment
import android.util.Log
import com.qualcomm.robotcore.eventloop.opmode.LinearOpMode
import com.qualcomm.robotcore.util.ElapsedTime

class LuaStdlib(val opmode: LinearOpMode)
{
	fun getDataDir(): String
	{
		return Environment.getExternalStorageDirectory().toString();
	}

	fun print(string: String)
	{
		Log.d("Lua", string);
	}

	fun err(msg: String)
	{
		throw LuaError(msg);
	}

	fun jniErr(msg: String)
	{
		throw JNIError(msg);
	}

	fun addData(label: String, msg: String)
	{
		opmode.telemetry.addData(label, msg);
	}

	fun update()
	{
		opmode.telemetry.update();
	}

	fun delay(time: Double): Boolean
	{
		val e = ElapsedTime();
		e.reset();
		while(e.seconds() < time && opmode.opModeIsActive());

		return !opmode.opModeIsActive();
	}

	fun checkRunning(): Boolean
	{
		return !opmode.opModeIsActive();
	}

	fun stop(): Boolean
	{
		return true;
	}
}