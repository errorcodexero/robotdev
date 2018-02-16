#ifndef GRABBER_CONTROLLER_H
#define GRABBER_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"

class GrabberController {
public:
    /// \brief the presets that the grabber already knows about.
    enum class Preset {
	CLOSED,				///< Move the grabber to the 'closed' position
	OPEN,                           ///< Move the grabber to the 'open' position
	STOWED                          ///< Move the grabber to the 'stowed' position
    };

    /// \brief create the grabber controller object
    GrabberController();

    /// \brief set the params object used to extract parameters from the params file
    void setParams(paramsInput* input_params);

    /// \brief get the params object used to extract parameters from the params file
    /// \returns the params object
    paramsInput* getParams();

    /// \brief move the grabber to a specific angle
    /// \param angle the angle in degrees for the grabber
    /// \param the start time of this operation
    void moveToAngle(double angle, double time);

    /// \brief move the grabber to a preset position
    /// \param preset the preset for the grabber angle
    /// \param time the start time of this operation
    void moveToAngle(Preset preset, double time);

    /// \brief this method is called each time the robot loop is run to update the grabber
    /// This method uses a PID controller to position the grabber at the desired angle
    /// \param angle the current angle of the grabber
    /// \param time the current time in seconds
    /// \param dt the time that has elapsed since the last time this was called
    /// \param out the output voltage for the grabber motor
    void update(double angle, double time, double dt, double &out);

    /// \brief this method is called when the grabber is idle
    /// \param angle the current height for the grabber
    /// \param time the current time
    /// \param dt the time since the last this this was called
    void idle(double angle, double time, double dt);

    /// \brief this method updates the angle target internally when a new target is required
    /// \param angle the new requested angle in inches
    /// \param time the start time of the new request
    void updateAngleOnChange(double angle, double time);
    
    /// \brief this method updates the angle target internally when a new target is required
    /// \param preset the preset we need to hit
    /// \param time the start time of the new request
    void updateAngleOnChange(Preset preset, double time);

    /// \brief returns true when the grabber has reached its desired angle
    /// \returns true when the grabber has reached its desired angle
    bool done();

    /// \brief sets mDoneCalibrating to the parameter
    /// \param done whether the grabber has finished calibrating
    void setDoneCalibrating(bool done);

    /// \brief returns whether the grabber has finished calibrating
    /// \returns true if the grabber has finished calibrating
    bool getDoneCalibrating();

    /// \brief returns the angle associated with a preset
    /// \param preset the preset of interest
    double presetToAngle(Preset preset);

private:
    // Indicates the mode of the grabber
    enum class Mode {
	IDLE,			// Doing nothing
        ANGLE			// Seeking a desired angle
    };

    //
    // The current mode for the grabber
    //
    Mode mMode;

    //
    // The target angle
    //
    double mTarget;

    //
    // The last target value that was applied
    //
    double mLastTarget;

    //
    // The threshold we use to consider an angle target met
    //
    double mAngleThreshold;

    //
    // The start time of an angle seeking operation
    //
    double mStartTime ;

    //
    // The PID controller for seeking a given angle
    //
    PIDCtrl mAnglePID;

    //
    // The global input params object
    //
    paramsInput* mInputParams;

    //
    // The last voltage returned for the motor
    //
    double mLastVoltage;

    //
    // The maximum change in volts per second for the motor voltage
    //
    double mMaxChange;

    //
    // If true, we have finished an angle operation and we are continuing to
    // dump data about the grabber angle.  This gives us the ability to track
    // any drift we may see after the operation is complete.
    bool mDataDumpMode;

    //
    // The start time for dumping data.  We only dump data after an operation for
    // a fixed amount of time (e.g. 5 seconds).
    //
    double mDataDumpStartTime;

    //
    // Represents whether or not the grabber has finished calibration
    //
    bool mDoneCalibrating;
};

#endif
