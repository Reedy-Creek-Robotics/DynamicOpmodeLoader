package org.firstinspires.ftc.teamcode.opmodeloader

import java.lang.RuntimeException

class LuaError : RuntimeException
{
	private var msg: String = "";
	
	constructor(m: String)
	{
		msg = m;
	}
	
	override fun getLocalizedMessage(): String
	{
		return msg;
	}
}