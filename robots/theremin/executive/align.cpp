#include "align.h"

#include <iostream>
#include <cmath>

using namespace std;

ostream& operator<<(ostream& o,Align::Mode a){
	switch(a){
		case Align::Mode::VISION:
			return o << "VISION";
		case Align::Mode::NONVISION:    
			return o << "NON-VISION";
		default:
			assert(0);
	}
}

static const Time INITIAL_SEARCH = 0.1;

Align::Align(Rad a, bool d):mode(Align::Mode::VISION),demo(d),blocks({}),current(0),estimated_angle(a),nonvision_align(Step{Rotate(a)}){
	initial_search.set(INITIAL_SEARCH);
}

Align::Align(Rad a):mode(Align::Mode::VISION),demo(0),blocks({}),current(0),estimated_angle(a),nonvision_align(Step{Rotate(a)}){
	initial_search.set(INITIAL_SEARCH);
}
Align::Align():Align(0){}

Block_pr::Block_pr(Pixy::Block b, double l, double r):block(b),left(l),right(r){}
Block_pr::Block_pr():Block_pr({0,0,0,0,0},0.0,0.0){}
Block_pr::Block_pr(const Pixy::Block b):block(b),left(Block_pr::generate_left(b.x)),right(Block_pr::generate_right(b.x)){}

ostream& operator<<(ostream& o, Block_pr const& a){
	o<<"Block_pr(";
	o<<"block:("<<a.block<<")";
	o<<" left:"<<a.left;
	o<<" right:"<<a.right;
	o<<")";
	return o;
}

double Block_pr::generate_pr(const int PIXELS_OFF){
	//returns a probability value between 0.0 and 1.0
	return pow(M_E,(-pow((double)PIXELS_OFF, 2.0) / 130.0 )); //arbitrary function which works pretty well (probabilty decreases further from 0px)
}
	
double old(const int PIXELS_OFF){
	const int WIDTH = (int)(Pixy::Block::max_x / Camera::FOV); 
	if(PIXELS_OFF < WIDTH * -4.5){
		return 0.0;
	}
	if(PIXELS_OFF < WIDTH * -3.5){
		return 0.04;
	} 
	if(PIXELS_OFF < WIDTH * -2.5){
		return 0.07;
	} 
	if(PIXELS_OFF < WIDTH * -1.5){
		return 0.10;
	} 
	if(PIXELS_OFF < WIDTH * -0.5){
		return 0.16;
	} 
	if(PIXELS_OFF < WIDTH * 0.5){
		return 0.26;
	} 
	if(PIXELS_OFF < WIDTH * 1.5){
		return 0.16;
	} 
	if(PIXELS_OFF < WIDTH * 2.5){
		return 0.10;
	}
	if(PIXELS_OFF < WIDTH * 3.5){
		return 0.07;
	} 
	if(PIXELS_OFF < WIDTH * 4.5){
		return 0.04;
	}
	return 0.0;
}

double Block_pr::generate_right(const int POS){
	//POS is px
	return generate_pr(Block_pr::RIGHT - POS);
}

double Block_pr::generate_left(const int POS){
	//POS is px
	return generate_pr(Block_pr::LEFT - POS);
}

void Align::update(Camera camera){
	/*if((!camera.enabled || camera.blocks.empty()) && initial_search.done()){
		//TODO: should search for vision target instead
		mode = Mode::NONVISION;
	} else {*/
		mode = Mode::VISION;
		blocks = camera.blocks;
		//TODO: check for two largest blocks that are in the expected y range
		if(blocks.size()>=2) current = mean(blocks[0].x,blocks[1].x);
		else if(blocks.size()==1) current = blocks[0].x;
		//if we dont see any blobs we will continue as if we still see what we saw before there being no blocks
	//}
	#ifdef PRINT_OUTS
	mode = Mode::VISION;
	blocks = camera.blocks;
	vector<Block_pr> block_prs;
	for(unsigned i = 0; i < blocks.size(); i++){
		Pixy::Block block = blocks[i];
		block_prs.push_back({block});
	}

	static const double PR_THRESHOLD = 0.001;

	Maybe<Pixy::Block> right_block = [&]{
		Maybe<Block_pr> max;
		for(Block_pr a: block_prs){
			if(a.right > PR_THRESHOLD && (!max || a.right > (*max).right)){
				max = a;
			}
		}
		if(!max) return Maybe<Pixy::Block>{};
		return Maybe<Pixy::Block>{(*max).block};
	}();

	Maybe<Pixy::Block> left_block = [&]{
		Maybe<Block_pr> max;
		for(Block_pr a: block_prs){
			if(a.left > PR_THRESHOLD && (!max || a.left > (*max).left)){
				max = a;
			}
		}
		if(!max) return Maybe<Pixy::Block>{};
		return Maybe<Pixy::Block>{(*max).block};
	}();

	static const int OFFSET = DIST_BETWEEN / 2;
	if(left_block && right_block){
		current = mean((*left_block).x,(*right_block).x);
	} else if(left_block){
		current = (*left_block).x + OFFSET;
	} else if(right_block){
		current = (*right_block).x - OFFSET;
	} else {
		//TODO: what do we do if we don't see anything that could be the tape?
		//DO nothing: current is not updated and we use whatever we saw last (often times is just the 0 from the constructor)
	}
	/*
	{//for testing
		cout<<"\nAlign block probabilities: \n";
		for(Block_pr a: block_prs){
			cout<<a<<"\n";
		}
		cout<<"Assigned left:("<<left_block<<") Assigned_right:("<<right_block<<")   curr:"<<current<<"\n";
	}
	*/
	#endif
}

Step::Status Align::done(Next_mode_info info){
	update(info.in.camera);
	switch(mode){
		case Mode::VISION:
			{
				if(demo) return Step::Status::UNFINISHED;
				const Time TIMEOUT_TIME = 8; //10; //seconds since start of autonomous
				if(info.since_switch > TIMEOUT_TIME){ 
					return Step::Status::FINISHED_SUCCESS; //TODO: make failure (offseason)
				}
				in_range.update(info.in.now,info.in.robot_mode.enabled);
				const int VISION_THRESHHOLD = 10;//24*.75;//starting with something large; theoretically this could be as large at 30px and be ok.
				const bool ok_now=(current > CENTER - VISION_THRESHHOLD && current < CENTER + VISION_THRESHHOLD);
				if(!ok_now){
					static const Time FINISH_TIME = .50;
					in_range.set(FINISH_TIME);
				}
				return in_range.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
			}
		case Mode::NONVISION:
			return nonvision_align.done(info);
		default:
			assert(0);
	}
}

Toplevel::Goal Align::run(Run_info info){
	return run(info,{});
}

Toplevel::Goal Align::run(Run_info info,Toplevel::Goal goals){
	initial_search.update(info.in.now,info.in.robot_mode.enabled);
	update(info.in.camera);
	switch(mode){
		case Mode::VISION:
			{
				static const double MIN_SPEED = 0.05;
				double power = target_to_out_power([&]{//power for left side, right side is opposite of this
					double error=CENTER-current;
					static const double P=.0025;//not tuned!
					static const double LIMIT=.1;//not tuned!
					return clamp(error*P,-LIMIT,LIMIT);
				}(),MIN_SPEED);
				goals.drive = Drivebase::Goal::absolute(-power,power);
				return goals;
			}
		case Mode::NONVISION:
			return nonvision_align.run(info,goals);
		default:
			assert(0);
	}
}

unique_ptr<Step_impl> Align::clone()const{
	return unique_ptr<Step_impl>(new Align(*this));
}

bool Align::operator==(Align const& b)const{
	return mode == b.mode && blocks == b.blocks && current == b.current && in_range == b.in_range && estimated_angle == b.estimated_angle && initial_search == b.initial_search && nonvision_align == b.nonvision_align;
}


#ifdef ALIGN_TEST

#include "test.h"
#include "chain.h"
#include "teleop.h"

int main(){
	Executive align{Chain{
		Step{Align()},
		Executive{Teleop()}
	}};
	test_executive(align);


	{
		static const unsigned RANGE = 2 * CENTER;
		for(unsigned x : range(RANGE)){
			Block_pr a = {{0,x,0,0,0}};
			cout<<a<<" vs: (l:"<<old(Block_pr::LEFT - x)<<" r:"<<old(Block_pr::RIGHT - x)<<")\n";
		}
	}

	//align.done();
	//align.run();
	return 0;
}
#endif
