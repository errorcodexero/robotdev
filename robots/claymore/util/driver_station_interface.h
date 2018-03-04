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

    template<MSP430_option DIGITAL_OUTPUTS_A>
    friend bool operator==(Driver_station_output const& a, Driver_station_output<DIGITAL_OUTPUTS_A> const& b) {
        if(a.digital.size() != b.digital.size()){
                return false;
        }
        for(unsigned i = 0; i < a.digital.size();i++){
                if(a.digital[i]!=b.digital[i]){
                        return 0;
                }
        }
        return 1;
    }

    template<MSP430_option DIGITAL_OUTPUTS_A>
    friend bool operator!=(Driver_station_output const& a, Driver_station_output<DIGITAL_OUTPUTS_A> const& b) {
	return !(a==b);
    }

    friend std::ostream& operator<<(std::ostream& o,Driver_station_output<DIGITAL_OUTPUTS> a) {
        o<<"(";
        o<<"digital:";
        for(unsigned i=0;i<a.digital.size();i++){
                o<<a.digital[i];
        }
        return o<<")";
    }

    Driver_station_output():digital(0) {}
};

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
