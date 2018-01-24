#ifndef STEP_H
#define STEP_H

#include "executive.h"
#include "../util/motion_profile.h"
#include "../util/settable_constant.h"
#include "../util/robot_constants.h"

struct Step_impl;

class Step{
	public:
	enum class Status{UNFINISHED,FINISHED_SUCCESS,FINISHED_FAILURE};

	private:
	std::unique_ptr<Step_impl> impl;

	public:
	explicit Step(Step_impl const&);
	Step(Step const&);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Status done(Next_mode_info);
	Step_impl const& get()const;
	void display(std::ostream&)const;
	bool operator==(Step const&)const;
	bool operator<(Step const&)const;
};

std::ostream& operator<<(std::ostream&,Step const&);

struct Step_impl{
	virtual ~Step_impl();

	virtual Toplevel::Goal run(Run_info,Toplevel::Goal)=0;
	virtual Toplevel::Goal run(Run_info)=0;
	virtual Step::Status done(Next_mode_info)=0;//could try to make this const.
	virtual std::unique_ptr<Step_impl> clone()const=0;
	virtual void display(std::ostream&)const;
	virtual bool operator<(Step const&)const=0;
	virtual bool operator==(Step const&)const=0;
};

template<typename T>
struct Step_impl_inner:Step_impl{
	std::unique_ptr<Step_impl> clone()const{
		T const& t=dynamic_cast<T const&>(*this);
		return std::unique_ptr<Step_impl>(new T(t));
	}
	
	void display(std::ostream& o)const{
		o<<type(*(T*)this);
	}

	bool operator==(Step const& a)const{
		//Step_impl const& b=a.get();
		//if(type_index(typeid(*this))!=type_index
		T const& t=dynamic_cast<T const&>(a.get());
		return operator==(t);
	}

	virtual bool operator==(T const&)const=0;

	bool operator<(Step const&)const{
		nyi
	}
};

using Inch=double;

class Drive:public Step_impl_inner<Drive>{//Drives straight a certain distance
	Countdown_timer timer;
	
	public:
	explicit Drive(double);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Drive const&)const;
};

class Drive_straight:public Step_impl_inner<Drive_straight>{//Drives straight a certain distance
	Inch target_dist;
	Drivebase::Distances initial_distances;
	bool init;
	Motion_profile motion_profile;
	Countdown_timer in_range;	
	Countdown_timer stall_timer;
	
	Drivebase::Distances get_distance_travelled(Drivebase::Distances);//TODO: do this better

	public:
	explicit Drive_straight(Inch);
	explicit Drive_straight(Inch,double,double);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Drive_straight const&)const;
};

class MP_drive:public Step_impl_inner<MP_drive>{
	Inch target_distance;
	Settable_constant<Drivebase::Goal> drive_goal;
	
	public:
	explicit MP_drive(Inch);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(MP_drive const&)const;
};

class Navx_drive_straight:public Step_impl_inner<Navx_drive_straight>{
	Inch target_distance;
	double angle_i;
	Settable_constant<Drivebase::Goal> drive_goal;

	public:
	explicit Navx_drive_straight(Inch);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Navx_drive_straight const&)const;
};

class Ram:public Step_impl_inner<Ram>{//Drives straight a certain distance
	Inch target_dist;
	Drivebase::Distances initial_distances;
	bool init;
	Countdown_timer stall_timer;

	Drivebase::Distances get_distance_travelled(Drivebase::Distances);//TODO: do this better

	public:
	explicit Ram(Inch);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Ram const&)const;
};


class Wait: public Step_impl_inner<Wait>{//Either stops all operation for a given period of time or continues to run the same goals for that time
	Countdown_timer wait_timer;//seconds
	public:
	explicit Wait(Time);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Wait const&)const;
};

class Combo: public Step_impl_inner<Combo>{//Runs two steps at the same time
	Step step_a;
	Step step_b;
	public:
	explicit Combo(Step,Step);//the second step will overwrite goals from the first one if they both modify the same parts of the robot

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	void display(std::ostream& o)const;
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Combo const&)const;
};

struct Rotate: Step_impl_inner<Rotate>{//orients the robot to a certain angle relative to its starting orientation
	Rad target_angle;//radians,clockwise=positive
	Drivebase::Distances initial_distances;
	bool init;
	Drivebase::Distances side_goals;
	Motion_profile motion_profile;
	Countdown_timer in_range;

	Drivebase::Distances angle_to_distances(Rad);
	Drivebase::Distances get_distance_travelled(Drivebase::Distances);

	explicit Rotate(Rad);
	explicit Rotate(Rad,double,double);
	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Rotate const&)const;
};

struct Navx_rotate: Step_impl_inner<Navx_rotate>{//orients the robot to a certain angle relative to its starting orientation //TODO
	double target_angle;
	Settable_constant<Drivebase::Goal> drive_goal;

	explicit Navx_rotate(double);
	
	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Navx_rotate const&)const;
};

#endif
