package org.firstinspires.ftc.teamcode.opmodeloader

import com.acmerobotics.roadrunner.geometry.Pose2d
import com.acmerobotics.roadrunner.geometry.Vector2d
import org.firstinspires.ftc.teamcode.roadrunner.drive.SampleMecanumDrive
import org.firstinspires.ftc.teamcode.roadrunner.trajectorysequence.TrajectorySequence
import org.firstinspires.ftc.teamcode.roadrunner.trajectorysequence.TrajectorySequenceBuilder

class TrajectoryBuilder
{
	private val drive: SampleMecanumDrive;
	private lateinit var builder: TrajectorySequenceBuilder;

	constructor(drive2: SampleMecanumDrive)
	{
		drive = drive2;
	}

	fun getTrajectory(): TrajectorySequence
	{
		return builder.build();
	}

	fun pathErr(msg: String)
	{
		throw PathError(msg);
	}

	fun makeBuilder(x: Double, y: Double, heading: Double)
	{
		builder = drive.trajectorySequenceBuilder(Pose2d(x, y, Math.toRadians(heading)));
	}

	fun lineTo(x: Double, y: Double)
	{
		builder.lineTo(Vector2d(x, y));
	}

	fun splineTo(x: Double, y: Double, heading: Double)
	{
		builder.splineTo(Vector2d(x, y), Math.toRadians(heading));
	}

	fun lineToLinearHeading(x: Double, y: Double, heading: Double)
	{
		builder.lineToLinearHeading(Pose2d(x, y, Math.toRadians(heading)));
	}

	fun splineToLinearHeading(x: Double, y: Double, heading: Double, angle: Double)
	{
		builder.splineToLinearHeading(
			Pose2d(x, y, Math.toRadians(heading)), Math.toRadians(angle)
		);
	}

	fun lineToConstantHeading(x: Double, y: Double)
	{
		builder.lineToConstantHeading(Vector2d(x, y));
	}

	fun splineToConstantHeading(x: Double, y: Double, heading: Double)
	{
		builder.splineToConstantHeading(Vector2d(x, y), Math.toRadians(heading));
	}

	fun lineToSplineHeading(x: Double, y: Double, heading: Double)
	{
		builder.lineToSplineHeading(Pose2d(x, y, Math.toRadians(heading)));
	}

	fun splineToSplineHeading(x: Double, y: Double, heading: Double, angle: Double)
	{
		builder.splineToSplineHeading(
			Pose2d(x, y, Math.toRadians(heading)), Math.toRadians(angle)
		);
	}

	fun turn(ang: Double)
	{
		builder.turn(ang);
	}

	fun wait(time: Double)
	{
		builder.waitSeconds(time);
	}

	fun marker(string: String)
	{
		builder.addDisplacementMarker {
			callDisplacement(string);
		};
	}

	private external fun callDisplacement(string: String);
}