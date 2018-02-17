#include "WPILib.h"
#include "AHRS.h"
#include "main.h"
#include "dio_control.h"
#include "talon_srx_control.h"
#include "pump_control.h"
#include "navx_control.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#include "params_parser.h"
#include "message_logger.h"

using namespace std;

Joystick_data read_joystick(frc::DriverStation& ds,int port){
	//I don't know what the DriverStation does when port is out of range.
	Joystick_data r;
	{
		auto lim=ds.GetStickPOVCount(port);
		assert(lim>=0);
		for(unsigned int i=0;i<(unsigned)lim;i++){
			r.pov=ds.GetStickPOV(port,i);
		}
	}
	{
		auto lim=ds.GetStickAxisCount(port);
		assert(lim>=0);
		for(unsigned i=0;i<(unsigned)lim;i++){
			r.axis[i]=ds.GetStickAxis(port,i);
		}
	}
	auto lim=ds.GetStickButtonCount(port);
	assert(lim>=0);
	for(unsigned i=0;i<(unsigned)lim;i++){
		//if(buttons&(1<<i)) r.button[i]=1;
		r.button[i]=ds.GetStickButton(port,i+1);
	}
	return r;
}

int read_joysticks(Robot_inputs &r, frc::DriverStation& ds){
	for(unsigned i=0;i<r.JOYSTICKS;i++){
		r.joystick[i]=read_joystick(ds,i);
	}
	return 0;
}

/*
void setoutputs_joysticks(Robot_inputs &r){
	DriverStation *ds=DriverStation::GetInstance();
	if(unsigned i=0;i<r.JOYSTICKS;i++){
		r.joystick[i]=SetOutput(1,1);			 
	}
}
*/

/*DriverStationEnhancedIO &get_driver_station(){
	DriverStation *ds=DriverStation::GetInstance();
	if(!ds) return NULL;
	return ds->GetEnhancedIO();
}*/

/*int read_driver_station(Driver_station_input& r){
	DriverStation *ds=DriverStation::GetInstance();
	if(!ds) return 2048;
	//Cyprus board isn't supported and a replacement is not yet available.
	//DriverStationEnhancedIO &en=ds->GetEnhancedIO();
	for(unsigned i=0;i<r.ANALOG_INPUTS;i++){
		r.analog[i]=0;//en.GetAnalogIn(i+1);//Causing a LOT of printouts when the DS is disconnected

	}
	for(unsigned i=0;i<r.DIGITAL_INPUTS;i++){
		r.digital[i]=0;//en.GetDigital(i+1);//Same as above ^^
	}
	return 0;
}*/

//it might make sense to put this in the Robot_inputs structure.  
Volt battery_voltage(){
	auto &d=frc::DriverStation::GetInstance();
	//AnalogModule *am=AnalogModule::GetInstance(DriverStation::kBatteryModuleNumber);
	/*if(!d){
		return 18; //this should look surprising
		//but maybe should actually return NAN.
	}*/
	/*float f=am->GetAverageVoltage(DriverStation::kBatteryChannel);
	return f * (1680.0 / 1000.0);//copied from WPIlib's DriverStation.cpp*/
	return d.GetBatteryVoltage();
}

int demo(...){
	cerr<<"In demo\n";
	return 0;
}

string space_out(string s){
	return s+"                                     ";
}

template<typename USER_CODE>
class To_roborio
{
	//TODO: see if these still have to all be pointers or if there's some alternative w/ the roboRIO
	frc::Solenoid *solenoid[Robot_outputs::SOLENOIDS];
	DIO_controls digital_io;
	frc::VictorSP *pwm[Robot_outputs::PWMS];
	frc::Relay *relay[Robot_outputs::RELAYS];
	frc::AnalogInput *analog_in[Robot_inputs::ANALOG_INPUTS];
	int error_code;
	USER_CODE main;
	Talon_srx_controls talon_srx_controls;
	Navx_control navx_control;
	//DriverStationLCD *lcd;
	//NetworkTable *table;
	//Gyro *gyro;
	frc::PowerDistributionPanel *power;
	Pump_control pump_control;
	//frc::Compressor *compressor;
	frc::DriverStation& driver_station;
#ifdef NEED_PIXY_CAM
	Pixy::PixyUART uart;
	Pixy::PixyCam camera;
	bool cam_data_recieved;
#endif
	std::ofstream null_stream;
	paramsInput input_params;
public:
  To_roborio():error_code(0),navx_control(frc::SPI::Port::kMXP),driver_station(frc::DriverStation::GetInstance()),null_stream("/dev/null")
	{
		messageLogger &logger = messageLogger::get();
		logger.enable(messageLogger::messageType::error);
		logger.enable(messageLogger::messageType::warning);
		logger.enable(messageLogger::messageType::info);
		logger.enable(messageLogger::messageType::debug);

		power = new frc::PowerDistributionPanel();

		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			solenoid[i]=new frc::Solenoid(i);//don't know of any way to determine module number, so just take the default one.
			if(!solenoid[i]) error_code|=8;
		}
		talon_srx_controls.init();
#ifdef THEREMIN
		talon_srx_controls.set_inverted(2);
		talon_srx_controls.set_inverted(3);

#endif
	
#ifdef CLAYMORE
		talon_srx_controls.set_inverted(0);
		talon_srx_controls.set_inverted(1);
		talon_srx_controls.set_inverted(2);
#endif				
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			pwm[i]=new frc::VictorSP(i);
			if(!pwm[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			relay[i]=new frc::Relay(i);
			if(!relay[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
			analog_in[i]=new frc::AnalogInput(i);
			if(!analog_in[i]) error_code|=8;
		}

		if (!input_params.readFile("/home/lvuser/params.txt"))
			std::cout << "Parameters file read failed" << std::endl ;
		else
			std::cout << "Parmeters file read sucessfully" << std::endl ;
		
		Drivebase::drivebase_controller.setParams(&input_params);

		/*
		for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
			int r=digital_io[i].set_channel(i);
			if(r) error_code|=256;
			//digital_in[i]=new DigitalInput(i+1);
		}*/
		
		/*lcd=DriverStationLCD::GetInstance();
		if(!lcd) error_code|=512;*/

		//table = NetworkTable::GetTable("crio");
		/*gyro=new Gyro(1);
		if(gyro){
			//gyro->InitGyro();
		}else{
			//TODO: Note this somehow.
		}*/

		/*
		compressor=new frc::Compressor();
		if(compressor){
			//for now I'm assuming that this means that it will run automatically.  I haven't seen any documentation that says what this does though.
			compressor->Start();
		}else{
			error_code|=512;
		}
		*/
		
		//Slave
		
		cout<<"Initialization Complete."<<endl<<flush;
	}
	
	int read_analog(Robot_inputs &r){
		int error=0;
		for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
			if(analog_in[i]){
				r.analog[i]=analog_in[i]->GetVoltage();
			}else{
				error=64;
			}
		}
		return error;
	}

	DS_info read_ds_info(){
		DS_info ds_info;
		ds_info.connected=driver_station.IsDSAttached();
		frc::DriverStation::Alliance ds_alliance=driver_station.GetAlliance();
		ds_info.alliance=[&]{
			switch(ds_alliance){
				case frc::DriverStation::Alliance::kRed: return Alliance::RED;
				case frc::DriverStation::Alliance::kBlue: return Alliance::BLUE;
				case frc::DriverStation::Alliance::kInvalid: return Alliance::INVALID;
				default: assert(0);
			}
		}();
		ds_info.location=driver_station.GetLocation();
		return ds_info;
	}

	Navx_input read_navx(){
		return navx_control.get();
	}

#ifdef NEED_PIXY_CAM
	Camera read_camera(Robot_inputs /*r*/){
		Camera c;
		camera.enable();
		if(camera.isNewData()) {
			cam_data_recieved=true;
			c.blocks=camera.getBlocks();
		}
		c.enabled=cam_data_recieved;
		return c;
	}
#endif
  
	pair<Robot_inputs,int> read(Robot_mode robot_mode){
		int error_code=0;
		Robot_inputs r;
		r.robot_mode=robot_mode;
		r.now=frc::Timer::GetFPGATimestamp();	
		r.ds_info=read_ds_info();
		error_code|=read_joysticks(r, driver_station);
		error_code|=read_analog(r);
		//error_code|=read_driver_station(r.driver_station);
		r.current=read_currents();
		r.input_params=&input_params;
		return make_pair(r,error_code);
	}
	array<double,Robot_inputs::CURRENT> read_currents(){
		array<double,Robot_inputs::CURRENT> current;
		for(unsigned x = 0;x < current.size();x++){
			if(power){
				current[x] = power->GetCurrent(x);
			}else{
				current[x] = -9001;
			}
		}		
		return current;
	}
	int set_solenoid(unsigned i,Solenoid_output v){
		if(i>=Robot_outputs::SOLENOIDS) return 1;
		if(!solenoid[i]) return 2;
		solenoid[i]->Set(v);
		return 0;
	}

	int set_pwm(unsigned i,Pwm_output p){
		if(i>=Robot_outputs::PWMS) return 1;
		if(!pwm[i]) return 2;
		pwm[i]->Set(p);
		return 0;
	}

	int set_relay(unsigned i,Relay_output out){
		if(i>=Robot_outputs::RELAYS) return 1;
		if(!relay[i]) return 2;
		relay[i]->Set([=](){
			if(out==Relay_output::_00) return frc::Relay::kOff;
			if(out==Relay_output::_01) return frc::Relay::kReverse;
			if(out==Relay_output::_10) return frc::Relay::kForward;
			//Assuming RELAY_11
			return frc::Relay::kOn;
		}());
		return 0;
	}

	int set_outputs(Robot_outputs out,bool /*enabled*/){
		int error_code=0;
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			int r=set_pwm(i,out.pwm[i]);
			if(r) error_code|=2;
		}

		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			int r=set_solenoid(i,out.solenoid[i]);
			if(r) error_code|=16;
		}
		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			int r=set_relay(i,out.relay[i]);
			if(r) error_code|=32;
		}
		
		digital_io.set(out.digital_io);
	
		{	
			frc::Joystick panel(Panel::PORT);
			for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
				panel.SetOutput(i+1, out.driver_station.digital[i]);
			}
		}
		{
			Checked_array<bool,Robot_outputs::TALON_SRX_OUTPUTS> enable_all;
			for(unsigned int i=0; i<Robot_outputs::TALON_SRX_OUTPUTS; i++){
				enable_all[i]=true;
			}
			talon_srx_controls.set(out.talon_srx,enable_all); 
		}
		pump_control.set(out.pump);

		navx_control.set(out.navx);	

		return error_code;
	}
	

	void run(Robot_inputs in){
		//std::ostream print_stream=cout;//(in.ds_info.connected && (print_num%PRINT_SPEED)==0)?cout:null_stream;

#ifdef PRINT_TIME
		double start = frc::Timer::GetFPGATimestamp() ;
#endif
		
		Robot_outputs out=main(in/*,print_stream*/);
		
#ifdef PRINT_TIME
		double elapsed = frc::Timer::GetFPGATimestamp() - start  ;
		std::cout << "process control " << elapsed * 1000 << " msec" << std::endl ;
		
		start = frc::Timer::GetFPGATimestamp() ;
#endif
		
		int x=set_outputs(out,in.robot_mode.enabled);
		
#ifdef PRINT_TIME
		elapsed = frc::Timer::GetFPGATimestamp() - start ;
		std::cout << "set output " << elapsed * 1000 << " msec" << std::endl ;
#endif
		
		if(x) cout<<"x was:"<<x<<"\n";
	}
	
	void run(Robot_mode mode){
#ifdef PRINT_TIME
		double start = frc::Timer::GetFPGATimestamp() ;
		double elapsed , last ;
#endif
		
		pair<Robot_inputs,int> in1=read(mode);
		
#ifdef PRINT_TIME
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - start ;
		std::cout << "    read base inputs " << elapsed * 1000 << " msec" << std::endl ;
		elapsed = last ;
#endif
		
		Robot_inputs in=in1.first;

		in.navx=read_navx();
		
#ifdef PRINT_TIME
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		std::cout << "    navx " << elapsed * 1000 << " msec" << std::endl ;
		elapsed = last ;
#endif

		error_code|=in1.second;
		in.digital_io=digital_io.get();

#ifdef PRINT_TIME
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		std::cout << "    digital io inputs " << elapsed * 1000 << " msec" << std::endl ;
		elapsed = last ;
#endif		
		
		in.talon_srx=talon_srx_controls.get();

#ifdef PRINT_TIME
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		std::cout << "    talon srx inputs " << elapsed * 1000 << " msec" << std::endl ;
		elapsed = last ;
#endif
		
		in.pump=pump_control.get();
		
#ifdef PRINT_TIME
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		std::cout << "    pump inputs " << elapsed * 1000 << " msec" << std::endl ;
		elapsed = last ;
		
		elapsed = elapsed - start ;
		std::cout << "read inputs " << elapsed * 1000 << " msec" << std::endl ;
#endif
		
		run(in);

		/*              
		// Network Table update:
		enum DsMode_t { DS_OTHER = 0, DS_AUTO = 1, DS_TELE = 2 };
		DsMode_t dsMode = 
			(in.robot_mode.autonomous && in.robot_mode.enabled) ? DS_AUTO :
			(in.robot_mode.enabled) ? DS_TELE : DS_OTHER;
			table->PutBoolean ("isEnabled", in.robot_mode.enabled);
			table->PutNumber  ("dsMode",    dsMode);*/
	}
};

template<typename USER_CODE>
class Robot_adapter: public frc::SampleRobot{
	To_roborio<USER_CODE> u;

	double looptime = 0.05 ;

	void RobotInit(){}
	
	void Autonomous(void)
	{
		while(IsAutonomous() && IsEnabled()){
			//might need a loop here

			double start = frc::Timer::GetFPGATimestamp() ;
			Robot_mode mode;
			mode.autonomous=1;
			mode.enabled=IsEnabled();
			u.run(mode);

			double elapsed = frc::Timer::GetFPGATimestamp() - start ;
			if (elapsed < looptime)
				frc::Wait(looptime - elapsed) ;
			else
			{
				std::cout << "Loop exceeded loop time, actual " << elapsed * 1000 << " msec" << std::endl ;
				std::cout << std::endl << std::endl ;
			}
		}
	}

	void OperatorControl(void)
	{
		//should see what happens when we get rid of this loop.  
		while (IsOperatorControl() && IsEnabled())
		{
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);
			
			//should see what happpens when this wait is removed.
			frc::Wait(0.005);// Wait 5 ms so we don't hog CPU cycle time
		}
	}
	
	//Runs during test mode
	void Test(){
		while(IsTest() && IsEnabled()){
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);
			
			frc::Wait(0.005);
		}
	}
	
	void Disabled(){
		while(IsDisabled()){
			Robot_mode r;
			r.autonomous=IsAutonomous();
			u.run(r);

			frc::Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(Robot_adapter<Main>);
