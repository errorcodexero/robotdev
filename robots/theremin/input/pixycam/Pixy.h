#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

#define PIXY_MAX_SIGNATURE 7

namespace Pixy {

////////////////////////////////////////////////////////////////////////
//
// Pixy matches blocks of pixels matching a particular color.
// It returns a vector of the blocks found in each video frame,
// sorted from largest to smallest.  Pixy can be trained to
// recognize blocks of up to eight different colors, each
// identified by "signature" number (0..7).
//

struct Block {
	unsigned signature;
	unsigned x;
	unsigned y;
	unsigned width;
	unsigned height;

	static const unsigned min_x;
	static const unsigned max_x;
	static const unsigned min_y;
	static const unsigned max_y;

	std::string print() const;
};

std::ostream& operator << ( std::ostream& stm, const Pixy::Block& block );
bool operator==(Block const&,Block const&);

////////////////////////////////////////////////////////////////////////
//
// PixySerial is a pure-virtual interface to an implementation
// class operating on some type and instance of serial interface.
//

class PixySerial
{
public:
	PixySerial();
	virtual ~PixySerial();
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool isOpen() const = 0;
	virtual int getByte() = 0;
	virtual int getWord() = 0;
	virtual int putString( const std::string& str ) = 0;
};

////////////////////////////////////////////////////////////////////////
//
// PixyCam is an opaque interface to the underlying implementation
// which is an instance of PixyImpl.
//

class PixyImpl;

class PixyCam {
public:
	PixyCam( PixySerial& link );	// type and instance of serial link
	~PixyCam();
	
	bool enable();			// returns success/failure
	bool disable();			// returns success/failure
	bool isNewData();			// returns true when new data is available
	std::vector<Block> getBlocks();	// returns vector of blocks seen

private:
	PixyImpl *impl;
};

}; // namespace Pixy

