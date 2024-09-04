package org.firstinspires.ftc.teamcode.opmodeloader

enum class LuaType(val id: Int)
{
	Double(3), Bool(1), String(4), Void(0), CheckRun(-1);
}

class LuaFunctionBuilder
{
	/**
	 * sets the object to pull functions from to expose to lua
	 */
	external fun <T> setCurrentObject(thing: T);
	
	/**
	 * starts a new lua table for functions
	 */
	external fun newClass();
	
	/**
	 * ends the table
	 */
	external fun endClass(name: String);
	
	/**
	 * adds a function from java to lua
	 *
	 * if a class is active when this is called the function will be put in that table
	 * else the function will be set as a global
	 */
	fun addFun(name: String, rtnType: LuaType = LuaType.Void, argTypes: List<LuaType> = listOf(
		LuaType.Void
	))
	{
		var funSignature = "(";
		var argc = 0;
		for(type in argTypes)
		{
			if(type == LuaType.CheckRun)
				throw LuaError("function argument type cannot be void");
			if(type == LuaType.Void)
				continue;
			funSignature += typeToStr(type);
			argc++;
		}
		funSignature += ')';
		funSignature += typeToStr(rtnType);
		addFunction(name, funSignature, rtnType.id, argc);
	}
	
	private external fun addFunction(name: String, funSignature: String, rtnType: Int, argc: Int);
	
	private fun typeToStr(type: LuaType): String
	{
		if(type == LuaType.Double)
			return "D";
		if(type == LuaType.Bool)
			return "Z";
		if(type == LuaType.Void)
			return "V";
		if(type == LuaType.CheckRun)
			return "Z";
		if(type == LuaType.String)
			return "Ljava/lang/String;";
		return "what";
	}
}

