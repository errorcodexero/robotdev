#include <iostream>
#include <cassert>
#include <numeric>
#include <iomanip>
#include <cmath>

#include "motor_current_monitor.h"

MotorCurrentMonitor::MotorCurrentMonitor(unsigned int n_motors)
{
    assert(n_motors != UNSPECIFIED);
    n_motors_ = n_motors;
    max_current_ = UNSPECIFIED;
    n_measurements_to_average_ = 40;  // Default to 40 == (2 seconds / 50 ms loop)
    variance_threshold_ = 0.20; // Default to 20% variance
    total_per_motor_.resize(n_motors);
    assert(measurements_.empty());
}

void MotorCurrentMonitor::setMaxCurrent(double max_current)
{
    assert(max_current > 0);
    max_current_ = max_current;
}

void MotorCurrentMonitor::setMeasurementsToAverage(unsigned int n_measurements)
{
    assert(n_measurements != UNSPECIFIED);
    n_measurements_to_average_ = n_measurements;
}

void MotorCurrentMonitor::setVarianceThreshold(double variance_threshold)
{
    variance_threshold_ = variance_threshold;
}

void MotorCurrentMonitor::validate() const
{
    if (n_motors_ == 1) {
        assert(max_current_ != UNSPECIFIED);  // If only 1 motor monitored, current threshold must be specified because this is all we can check
    }
}

void MotorCurrentMonitor::logNewMeasurement(const Measurement motor_currents)
{
    validate();
    assert(n_motors_ == motor_currents.size());

    if (measurements_.size() < n_measurements_to_average_) {    // Still filling up queue of measurements
        for (unsigned int i=0; i<motor_currents.size(); ++i) {
            total_per_motor_[i] += motor_currents[i];
        }
    } else {   // Queue of measurements full. Add latest data, then subtract and remove oldest data points.
        assert(measurements_.size() == n_measurements_to_average_);
        const Measurement& oldest_measurement = measurements_.front();
        for (unsigned int i=0; i<motor_currents.size(); ++i) {
            total_per_motor_[i] = total_per_motor_[i] - oldest_measurement[i] + motor_currents[i];
        }
        measurements_.pop_front();
    }
    measurements_.push_back(motor_currents);
    checkViolation();
}

void MotorCurrentMonitor::checkViolation() const
{
    assert(measurements_.size() <= n_measurements_to_average_);

    // Only check if we have enough data logged
    if (measurements_.size() == n_measurements_to_average_)
    {
        double average = std::accumulate(total_per_motor_.begin(), total_per_motor_.end(), 0.0) / total_per_motor_.size() / measurements_.size();
        if (average != 0) {
            for (unsigned int i=0; i<total_per_motor_.size(); ++i) {
                double val = total_per_motor_[i] / measurements_.size();

                // Check if motor exceeds threshold
                if (max_current_ != UNSPECIFIED) {
                    if (val > max_current_) {
                        std::cout << "Motor " << i << " may have failed. ";
                        std::cout << std::setprecision(2) << std::fixed;
                        std::cout << "Current = " << val << ". ";
                        std::cout << "Threshold = " << max_current_ << "." << std::endl;
                    }
                }

                // Check variance between motors
                double deviation = std::fabs(val-average)/average;
                if (deviation > variance_threshold_) {
                    std::cout << "Motor " << i << " may have failed. ";
                    std::cout << std::setprecision(2) << std::fixed;
                    std::cout << "Current = " << val << ". ";
                    std::cout << "Average of motors = " << average << ". ";
                    std::cout << "(" << deviation*100.0 << "% > " << variance_threshold_*100.0 << "% threshold.)" << std::endl;
                }
            }
        }
    }
}


#ifdef MOTOR_CURRENT_MONITOR_TEST

int main()
{
    MotorCurrentMonitor m(2);
    m.setMeasurementsToAverage(2);
    std::cout << "Logging measurement #1" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 10} );
    std::cout << "Logging measurement #2" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 20} );
    std::cout << "Logging measurement #3" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 30} );
    std::cout << "Logging measurement #4" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 90} );

    m.setMaxCurrent(100);
    std::cout << "Logging measurement #5" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 90} );
    std::cout << "Logging measurement #6" << std::endl;
    m.logNewMeasurement( std::vector<double>{10, 200} );
    return 0;
}

#endif
