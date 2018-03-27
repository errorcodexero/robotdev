#include "drivecollect.h"


//
// Drive_and_collect: Drive forward and collect until a cube is collected
//

double Drive_and_collect::distance_travelled;

Drive_and_collect::Drive_and_collect() : Step("drivecollect")
{
	maxdistance = std::numeric_limits<double>::max() ;
}

Drive_and_collect::Drive_and_collect(double maxdist) : Step("drivecollect(dist)")
{
	maxdistance = maxdist ;
}

void Drive_and_collect::init(const Robot_inputs &in, Toplevel::Status_detail &status)
{
	paramsInput* input_params = paramsInput::get();

	initial_distances = status.drive.distances;
	timeout_timer.set(input_params->getValue("step:drive_and_collect:timeout", 5.0));
	mStart = in.now ;
}

Step::Status Drive_and_collect::done(Next_mode_info info)
{
	Drivebase::Distances distances_travelled = info.status.drive.distances - initial_distances;
	double dist = (distances_travelled.l + distances_travelled.r) / 2.0;

    Step::Status ret = Step::Status::UNFINISHED ;

	if (timeout_timer.done() ||
		dist > maxdistance ||
		Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
		ret = Step::Status::FINISHED_SUCCESS ;
	
    if (ret == Step::Status::FINISHED_SUCCESS) 
    {
		distance_travelled = (distances_travelled.l + distances_travelled.r) / 2.0;

		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Drive and collect step complete, elapsed " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Drive_and_collect::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Drive_and_collect::run(Run_info info,Toplevel::Goal goals)
{
	paramsInput* input_params = paramsInput::get();
	
	timeout_timer.update(info.in.now, true);
	double drive_power = input_params->getValue("step:drive_and_collect:drive_power", 0.4);
	goals.drive = Drivebase::Goal::absolute(drive_power, drive_power);
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
	goals.intake = Intake::Goal::in();
	
	if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube)
		goals.grabber = Grabber::Goal::clamp();

    return goals;
}
