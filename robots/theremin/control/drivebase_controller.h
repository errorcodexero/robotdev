#ifndef DRIVEBASE_CONTROLLER_H
#define DRIVEBASE_CONTROLLER_H

#include <list>
#include "pidctrl.h"
#include "params_parser.h"
#include "UdpBroadcastSender.h"

class DrivebaseController {
public:

    DrivebaseController();

	/// \brief set the parameters object
	/// This object is used to lookup parameters read from the parameters file
	/// \param pi_p pointer to the parameter input reader
    void setParams(paramsInput *pi_p);

	/// \brief initialize the drive controller to drive a fixed distance value
	/// \param dist the absolute distance to drive to
	/// \param angle the angle to maintain while driving
    void initDistance(double dist, double angle);

	/// \brief rotate to the given angle
	/// \param angle the angle to rotate to
    void initAngle(double angle);

	/// \brief update the left and right motor values to acheive the desired goal
	/// \param dl the distance traveled by the left wheel
	/// \param dr the distance traveled by the right wheel
	/// \param angle the current angle of the robot
	/// \param dt the time elapsed since the last time update was called
	/// \param time the absolute time value
	/// \param out_l return value for the left motor voltage
	/// \param out_r return value for the right motor voltage
    void update(double dl, double dr, double angle, double dt, double time, double &out_l, double &out_r);

	/// \brief called when the drivebase is idle
	/// \param dl the distance traveled by the left wheel
	/// \param dr the distance traveled by the right wheel
	/// \param angle the current angle of the robot
	/// \param dt the time elapsed since the last time update was called
	/// \param time the absolute time value
    void idle(double dl, double dr, double angle, double dt, double time);

	/// \brief returns true when the robot has reached its distance or angle goal
    bool done();
	
private:
    enum class Mode {
		IDLE,
		DISTANCE,
		ANGLE
    };

    //
    // The current mode of the drivebase.  Either trying to acheive a distance, or an
    // angle, or IDLE mode.  In IDLE mode the drivebase motors are controlled by the
    // drivebase.cpp code.
    //
    Mode mMode;

    //
    // The target distance or angle, depending on the mode of the drivebase_controller
    //
    double mTarget ;

    //
    // The starting angle, that we are trying to maintain, while driving straight
    //
    double mTargetCorrectionAngle;

    //
    // The threshold for declaring a distance target met.
    //
    double mDistanceThreshold;

    //
    // The threshold for declaring an angle target met
    //
    double mAngleThreshold ;

    //
    // The velocity threshold for declaring an angle target met.  The rotational
    // velocity of the robot must be below this value to consider the angle target
    // met.
    //
    double mAngleVThreshold;

    //
    // The PID controller for adjusting to the left and right motor to drive straight
    //
    PIDCtrl mStraightnessPid ;

    //
    // The PID controller for driving a given distance
    //
    PIDCtrl mDistPid ;

    //
    // The PID controller for rotating to a given distance
    //
    PIDCtrl mAnglePid;

    //
    // The angle of the robot during the last robot loop.  Used to calculate
    // the angular velocity of the robot
    //
    double mLastAngle;

    //
    // The distance travelled by the robot for the last N robot loops.  This
    // is used to determine if the robot is stalled
    //
    std::list<double> mDistanceHistory;

    //
    // The number of samples to keep to determine if the robot is stalled
    //
    size_t mNsamples;

    //
    // If true, ther PID controller for distance has been reset from the long distance
    // PID constants, to the local PID constants
    //
    bool mResetPid;

    //
    // The threshold below which, the drivebase is considered stalled and the
    // PID constants are reset to the local PID constants
    //
    double mPidResetThreshold;

    //
    // The input paramsters object.  Used to get all tunable parameters from a
    // file.
    //
    paramsInput* mInputParams;

    //
    // The last voltage applied to the motors, used to ensure we dont ramp the
    // voltage to the motors any faster than mMaxChange.
    //
    double mLastVoltage ;

    //
    // The maximum voltage change per second allowed on the motors
    //
    double mMaxChange ;

    //
    // If true continue to dump data while in the IDLE mode, used to detect drift
    //
    bool mDataDumpMode ;

    //
    // The start time for dumping data in IDLE mode, allows us to limit the IDLE data
    // dump to a five second time period
    //
    double mDataDumpStartTime ;
};

#endif
