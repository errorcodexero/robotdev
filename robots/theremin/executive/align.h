#ifndef ALIGN_H
#define ALIGN_H

#include "executive.h"
#include "../util/motion_profile.h"
#include "step.h"

static const int CENTER = 145; //px posiiton in the image that corresponds to the center of the robot (calibrated to account for angle and offset of the camera)
static const int DIST_BETWEEN = 70; //px the expected distance between the two pieces of tape if the robot is 2ft away

struct Block_pr{
	static const int LEFT = CENTER - (DIST_BETWEEN / 2); //px where we expect the left tape to be
	static const int RIGHT = CENTER + (DIST_BETWEEN / 2); //px where we expect the right tape to be
	
	Pixy::Block block;
	double left;
	double right;
	
	static double generate_left(const int);
	static double generate_right(const int);
	static double generate_pr(const int);
	
	Block_pr();
	Block_pr(Pixy::Block,double,double);
	Block_pr(const Pixy::Block);
};

std::ostream& operator<<(std::ostream&,Block_pr const&);

struct Align: public Step_impl_inner<Align>{
	enum class Mode{VISION,NONVISION};
	Mode mode;
	bool demo;
	std::vector<Pixy::Block> blocks;
	int current;
	Countdown_timer in_range;	
	Countdown_timer initial_search;
	
	Rad estimated_angle;
	Step nonvision_align;
	
	void update(Camera);

	public:
	explicit Align(Rad, bool);
	explicit Align(Rad); //TODO: pass in initial encoders or find some way to set them inside of the Turn step
	explicit Align();

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Align const&)const;
};
#endif
