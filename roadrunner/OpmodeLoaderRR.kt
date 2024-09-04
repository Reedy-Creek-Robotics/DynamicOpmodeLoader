package org.firstinspires.ftc.teamcode.opmodeloader

import com.qualcomm.robotcore.eventloop.opmode.LinearOpMode
import com.qualcomm.robotcore.util.ElapsedTime
import org.firstinspires.ftc.robotcore.external.Telemetry
import org.firstinspires.ftc.teamcode.roadrunner.drive.SampleMecanumDrive
import org.firstinspires.ftc.teamcode.roadrunner.trajectorysequence.TrajectorySequence

class OpmodeLoaderRR(drive2: SampleMecanumDrive, opmode: LinearOpMode) : OpmodeLoader(opmode)
{
	private val telemetry: Telemetry;
	private val drive: SampleMecanumDrive = drive2;
	private val builder: TrajectoryBuilder = TrajectoryBuilder(drive);
	private val trajectories: java.util.ArrayList<TrajectorySequence> = java.util.ArrayList(3);

	init
	{
		telemetry = opmode.telemetry
	}

	/**
	 * Builds the roadrunner paths
	 */
	override fun loadOpmode(name: String)
	{
		opmodeName = name;

		internalInit(builder);

		telemetry.addLine("building path 1");
		telemetry.update();

		buildPath(name, 0);
		trajectories.add(builder.getTrajectory());

		telemetry.addLine("building path 2");
		telemetry.update();

		buildPath(name, 1);
		trajectories.add(builder.getTrajectory());

		telemetry.addLine("building path 3");
		telemetry.update();

		buildPath(name, 2);
		trajectories.add(builder.getTrajectory());

		telemetry.addLine("done");
		telemetry.update();

		close();
	}
	
	/**
	 * starts the opmode and repeatedly calls init
	 */
	override fun startLoop(recognition: Int)
	{
		drive.poseEstimate = trajectories[recognition].start();
		start(recognition);
		drive.followTrajectorySequenceAsync(trajectories[recognition]);
		val elapsedTime = ElapsedTime();
		var prevTime = 0.0;
		while(drive.isBusy && opmode.opModeIsActive())
		{
			val curTime = elapsedTime.seconds();
			val deltaTime = curTime - prevTime;
			prevTime = curTime;
			drive.update();
			update(deltaTime, curTime);
		}
	}

	private external fun buildPath(name: String, recognition: Int);
	private external fun internalInit(builder: TrajectoryBuilder);
	private external fun close();
}