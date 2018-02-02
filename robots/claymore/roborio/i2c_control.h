#ifndef I2C_CONTROL_H
#define I2C_CONTROL_H

#include "I2C.h"
#include "../util/interface.h"
#include "../util/checked_array.h"

class I2C_control{
	private:
	frc::I2C *i2c;
	
	public:
	#define I2C_PORTS X(ONBOARD) X(MXP)
	enum class I2C_port{
		#define X(NAME) NAME,
		I2C_PORTS
		#undef X
	};

	private:
	I2C_port i2c_port;
	
	public:
	I2C_control();
	~I2C_control();
	I2C_control(I2C_port port,int address);
	
	void init(I2C_port port,int address);
	void write(uint8_t*,int)const;
	void read(uint8_t*,int)const;
	friend std::ostream& operator<<(std::ostream&,I2C_control);
};

#endif