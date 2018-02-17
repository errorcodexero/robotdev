#ifndef LIFTER_CONTROLLER_H
#define LIFTER_CONTROLLER_H

#include "pidctrl.h"
#include "params_parser.h"

class LifterController {
public:
    // Butch - do we need switch low and switch high?
    //         do we need scale low and scale high?
    
    /// \brief the presets that the lifter already knows about.
    enum class Preset {
	FLOOR,				///< Move the lifter to the floor
	EXCHANGE,			///< Move the lifter to the exchange
	SWITCH,				///< Move the lifter to the height of the switch
        SCALE,				///< Move the lifter to the height of the scale
	PREP_CLIMB			///< Move the lifter to the climb position
    };

    /// \brief create the lift controller object
    LifterController();

    /// \brief set the params object used to extract parameters from the params file
    void setParams(paramsInput* input_params);

    /// \brief get the params object used to extract parameters from the params file
    /// \returns the params object
    paramsInput* getParams();

    /// \brief move the lifter to a specific height
    /// \param height the height in inches for the lifter
    /// \param current_height the current lifter height
    /// \param the start time of this operation
    void moveToHeight(double height, double current_height, double time);

    /// \brief move the lifter to a preset position
    /// \param preset the preset for the lifter height
    /// \param current_height the current lifter height
    /// \param time the start time of this operation
    void moveToHeight(Preset preset, double current_height, double time);

    /// \brief move the lifter to a preset position in the background
    /// \param preset the preset for the lifter height
    /// \param current_height the current lifter height
    /// \param time the start time of this operation
    void backgroundMoveToHeight(Preset preset, double current_height, double time);

    /// \brief put the lifter into calibration mode
    void calibrate();

    /// \brief this method is called each time the robot loop is run to update the lifer
    /// This method uses a PID controller to position the lifter to the desired location
    /// \param height the current height of the lifter
    /// \param time the current time in seconds
    /// \param dt the time that has elapsed since the last time this was called
    /// \param out the output voltage for the lifter motor
    void update(double height, double time, double dt, double &out);

    /// \brief this method is called when the lifter is idle
    /// \param height the current height for the lifter
    /// \param time the current time
    /// \param dt the time since the last this this was called
    void idle(double height, double time, double dt);

    /// \brief this method updates the height target internally when a new target is required
    /// \param height the new requeste height in inches
    /// \param time the start time of the new request
    void updateHeightOnChange(double height, double current_height, double time);
    
    /// \brief this method updates the height target internally when a new target is required
    /// \param preset the preset we need to hit
    /// \param time the start time of the new request
    void updateHeightOnChange(Preset preset, double current_height, double time);

    /// \brief returns true when the lifter has reached its desired height
    /// \returns true when the lifter has reached its desired height
    bool done();

    /// \brief return true when in the lifter is running in the background
    /// \returns true when the lifter is running in the background
    bool runningInBackground();

    /// \brief return true when the lifter is calibrating
    /// \returns true when the lifter is calibrating
    bool calibrating();

    /// \brief returns the height assocaited with a preset
    /// \param preset the preset of interest
    double presetToHeight(Preset preset);

private:
    // Indicates the mode of the lifter
    enum class Mode {
	IDLE,			// Doing nothing
        HEIGHT,			// Seeking a desired height
	BACKGROUND,             // Going to a height in the background
	CALIBRATING             // Calibrating the lifter 
    };

    //
    // The current mode for the lifter
    //
    Mode mMode;

    //
    // The target height
    //
    double mTarget;

    //
    // The last target value that was applied
    //
    double mLastTarget;

    //
    // The threshold we use to consider a height target met
    //
    double mHeightThreshold;

    //
    // The start time of a height seeking operation
    //
    double mStartTime ;

    //
    // The PID controller for seeking a given height
    //
    PIDCtrl mHeightPID;

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
    // If true, we have finished a height operation and we are continuing to
    // dump data about the lifter height.  This gives us the ability to track
    // any drift we may see after the operation is complete.
    bool mDataDumpMode;

    //
    // The start time for dumping data.  We only dump data after an operation for
    // a fixed amount of time (e.g. 5 seconds).
    //
    double mDataDumpStartTime;

};

#endif
