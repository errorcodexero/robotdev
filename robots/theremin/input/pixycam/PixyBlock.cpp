#include "Pixy.h"
#include <sstream>

namespace Pixy {

const unsigned Block::min_x = 0;
const unsigned Block::max_x = 319;
const unsigned Block::min_y = 0;
const unsigned Block::max_y = 199;

std::string Block::print() const
{
	std::ostringstream stm;
	stm << *this;
	return stm.str();
}

std::ostream& operator<< ( std::ostream& stm, const Block& block )
{
	char buf[100];
	// use snprintf for easy formatting
	snprintf(buf, sizeof buf, "sig %4d   x %4d   y %4d   width %4d   height %4d",
	block.signature, block.x, block.y, block.width, block.height);
	stm << buf;
	return stm;
}

bool operator==(Block const& a,Block const& b){
	#define X(name) a.name==b.name &&
	return X(signature) X(x) X(y) X(width) X(height) true;
	#undef X
}

}; // namespace Pixy

