package org.firstinspires.ftc.teamcode.opmodeloader

import com.qualcomm.robotcore.eventloop.opmode.LinearOpMode
import com.qualcomm.robotcore.util.ElapsedTime
import com.acmerobotics.dashboard.config.Config

@Config
open class OpmodeLoader(opmode2: LinearOpMode)
{
	companion object
	{
		init
		{
			System.loadLibrary("ftcrobotcontroller");
		}
		
		@JvmField
		val defaultRecognition: Int = 0;
	}
	
	protected val opmode = opmode2;
	private val stdlib = LuaStdlib(opmode2);
	private val builder = LuaFunctionBuilder();
	protected var opmodeName: String = "'";
	
	/**
	 * returns a function builder object for exposing functions to lua
	 */
	fun getFunctionBuilder(): LuaFunctionBuilder
	{
		return builder;
	}
	
	/**
	 * initalizes the lua instance and returns a list with all of the opmode names
	 */
	fun init(): Array<String>
	{
		val opmodes = internalInit(stdlib);

		builder.setCurrentObject(stdlib);

		builder.addFun("print", LuaType.Void, listOf(LuaType.String));
		builder.addFun("delay", LuaType.CheckRun, listOf(LuaType.Double));
		builder.addFun("checkRunning", LuaType.CheckRun);
		builder.addFun("stop", LuaType.CheckRun);

		builder.newClass();
		builder.addFun("addData", LuaType.Void, listOf(LuaType.String, LuaType.String));
		builder.addFun("update", LuaType.Void);
		builder.endClass("telem");

		return opmodes;
	}
	
	/**
	 * loads an opmode to be run
	 */
	open fun loadOpmode(name: String)
	{
		opmodeName = name;
	}
	
	/**
	 * starts the opmode using the default recognition value
	 */
	fun start()
	{
		start(opmodeName, defaultRecognition);
	}
	
	/**
	 * starts the opmode
	 */
	fun start(recognition: Int)
	{
		start(opmodeName, recognition);
	}
	
	/**
	 * starts the opmode using the default recognition value and repeatedly calls update
	 */
	fun startLoop()
	{
		startLoop(defaultRecognition);
	}
	
	/**
	 * starts the opmode and repeatedly calls update
	 */
	open fun startLoop(recognition: Int)
	{
		start(opmodeName, recognition)

		val elapsedTime = ElapsedTime();
		elapsedTime.reset();
		var prevTime = 0.0;

		while(opmode.opModeIsActive())
		{
			val curTime = elapsedTime.seconds();
			val deltaTime = curTime - prevTime;
			if(update(deltaTime, curTime))
				return;
			prevTime = curTime;
		}
	}
	
	private external fun internalInit(luaStdlib: LuaStdlib): Array<String>;
	private external fun start(name: String, recognition: Int);
	external fun update(deltaTime: Double, elapsedTime: Double): Boolean;
}