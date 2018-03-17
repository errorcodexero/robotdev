#pragma once

#include <vector>
#include <list>
#include <limits>


///
/// Pass in set of motor currents at periodic interval.
/// On average, the motors should consume similar amounts of current.
/// If running average deviates from expectation, log an error - may
/// have a bad motor.
///

class MotorCurrentMonitor {
    const unsigned int UNSPECIFIED = std::numeric_limits<unsigned int>::max();
    typedef std::vector<double> Measurement;
    typedef std::list<Measurement> Measurements;

    public:
    MotorCurrentMonitor(unsigned int n_motors);
    void setMaxCurrent(double max_current);
    void setMeasurementsToAverage(unsigned n_measurements);
    void setVarianceThreshold(double variance_threshold);
    void validate() const;
    void logNewMeasurement(const std::vector<double> motor_currents);
    void checkViolation() const;

    private:
    unsigned int n_motors_;
    unsigned int n_measurements_to_average_;
    unsigned int n_available_measurements_;
    double variance_threshold_;
    double max_current_;
    Measurement total_per_motor_;
    Measurements measurements_;

};
