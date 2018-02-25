#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include "maybe.h"

//this is the interface for the OI

enum MSP430_option{_1 = 11, _2 = 6, _3 = 2};

template<MSP430_option DIGITAL_OUTPUTS>
struct Driver_station_output{
    std::bitset<DIGITAL_OUTPUTS> digital;

    Driver_station_output();
};

template<MSP430_option DIGITAL_OUTPUTS_A,MSP430_option DIGITAL_OUTPUTS_B>
bool operator==(Driver_station_output<DIGITAL_OUTPUTS_A>,Driver_station_output<DIGITAL_OUTPUTS_B>);
template<MSP430_option DIGITAL_OUTPUTS_A,MSP430_option DIGITAL_OUTPUTS_B>
bool operator!=(Driver_station_output<DIGITAL_OUTPUTS_A>,Driver_station_output<DIGITAL_OUTPUTS_B>);
template<MSP430_option DIGITAL_OUTPUTS>
std::ostream& operator<<(std::ostream&,Driver_station_output<DIGITAL_OUTPUTS>);

struct Driver_station_input{
    static const unsigned ANALOG_INPUTS=8;//TODO get real value
    std::array<double,ANALOG_INPUTS> analog;

    static const unsigned DIGITAL_INPUTS=8;//TODO get real value
    std::bitset<DIGITAL_INPUTS> digital;

    Driver_station_input();

    static Maybe<Driver_station_input> parse(std::string const&);
    static Driver_station_input rand();
};
bool operator<(Driver_station_input,Driver_station_input);
bool operator==(Driver_station_input,Driver_station_input);
bool operator!=(Driver_station_input,Driver_station_input);
std::ostream& operator<<(std::ostream&,Driver_station_input);
