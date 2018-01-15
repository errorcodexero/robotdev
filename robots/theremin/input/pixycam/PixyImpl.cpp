#include "Pixy.h"
#include <thread>
#include <mutex>
#include <pthread.h>

namespace Pixy {

PixySerial::PixySerial() {};
PixySerial::~PixySerial() {};

#define PIXY_START_WORD             0xaa55
#define PIXY_START_WORD_CC          0xaa56
#define PIXY_START_WORDX            0x55aa

class PixyImpl
{
public:
	PixyImpl( PixySerial& link );
	~PixyImpl();
	
	bool enable();
	bool disable();	
	bool isNewData();
	std::vector<Block> getBlocks();

private:
	// link management
	PixySerial& m_link;

	// thread management
	std::thread m_monitor;
	std::mutex m_mutex;
	bool m_enabled;
	volatile bool m_interrupt;

	// parser state
	bool m_skipStart;
	enum BlockType { NORMAL_BLOCK, CC_BLOCK } m_blockType;

	// results
	std::vector<Block> m_blocks;
	volatile bool m_newData;

	void run(void);
	void updateBlocks( const std::vector<Block>& blocks );
	std::vector<Block> getFrame(void);
	bool getStart();
};

PixyImpl::PixyImpl( PixySerial& link ): m_link(link),
	m_monitor(),
	m_mutex(),
	m_enabled(false),
	m_interrupt(false),
	m_skipStart(false),
	m_blockType(NORMAL_BLOCK),
	m_blocks(),
	m_newData(false)
{
	;
}

PixyImpl::~PixyImpl()
{
	disable();
};

bool PixyImpl::enable()
{
	if (!m_enabled) {
		if (!m_link.open()) {
			return false;
		}
		m_interrupt = false;
		m_monitor = std::thread( [=] { run(); } );
		m_enabled = true;
	}
	return true;
}

bool PixyImpl::disable()
{
	if (m_enabled) {
		m_interrupt = true;
		m_monitor.join();
		m_link.close();
	}
	m_interrupt = false;
	m_enabled = false;
	m_blocks.clear();
	m_newData = false;
	return true;
}

bool PixyImpl::isNewData()
{
	return m_newData;
}

std::vector<Block> PixyImpl::getBlocks()
{
	// return a COPY of the current block list
	// clear the newData flag
	std::vector<Block> ret;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ret = m_blocks;
		m_newData = false;
	}

	return ret;
}

void PixyImpl::updateBlocks( const std::vector<Block>& newBlocks )
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_blocks = newBlocks;
	m_newData = true;
}

void PixyImpl::run()
{
	m_skipStart = false;
	m_blockType = NORMAL_BLOCK;
	while (!m_interrupt) {
		std::vector<Block> newBlocks = getFrame();
		updateBlocks(newBlocks);
	}
}

std::vector<Block> PixyImpl::getFrame()
{
#ifdef TRACE
	std::cerr << "PixyImpl::getFrame" << std::endl;
#endif

	std::vector<Block> blocks;

	if (m_skipStart) {
		m_skipStart = false;
		} else {
		if (!getStart()) {
			return blocks;
		}
	}
#ifdef TRACE
	std::cerr << " SYNC" << std::endl;
#endif

	for (;;) {
		uint16_t check = m_link.getWord();
		if (check == PIXY_START_WORD) {
			// we've reached the beginning of the next frame
#ifdef TRACE
			std::cerr << " START" << std::endl;
#endif
			m_skipStart = true;
			m_blockType = NORMAL_BLOCK;
			break;
		}
		else if (check == PIXY_START_WORD_CC) {
			// we've reached the beginning of the next frame
#ifdef TRACE
			std::cerr << " START_CC" << std::endl;
#endif
			m_skipStart = true;
			m_blockType = CC_BLOCK;
			break;
		}
		else if (check == 0) {
#ifdef TRACE
			std::cerr << " END" << std::endl;
#endif
			break;
		}

		Block block;
		block.signature = m_link.getWord();
		block.x = m_link.getWord();
		block.y = m_link.getWord();
		block.width = m_link.getWord();
		block.height = m_link.getWord();

		uint16_t sum = block.signature + block.x + block.y + block.width + block.height;
		if (sum != check) {
#ifdef TRACE
			std::cerr << " CKSUM_ERR: was " << std::hex << sum << std::dec << " expected " << std::hex << check << std::dec << std::endl;
#endif
			break;
		}
#ifdef TRACE
		std::cerr << " BLOCK" << std::endl;
#endif
		blocks.push_back(block);

		uint16_t sync = m_link.getWord();
		if (sync == PIXY_START_WORD) {
			// we've reached the beginning of the next block
#ifdef TRACE
			std::cerr << " START" << std::endl;
#endif
			m_blockType = NORMAL_BLOCK;
			continue;
		}
		else if (sync == PIXY_START_WORD_CC) {
			// we've reached the beginning of the next block
#ifdef TRACE
			std::cerr << " START_CC" << std::endl;
#endif
			m_blockType = CC_BLOCK;
			continue;
		}
		else {
			// protocol error
#ifdef TRACE
			std::cerr << " ERR" << std::endl;
#endif
			m_skipStart = false;
			break;
		}
	}

	return blocks;
}

bool PixyImpl::getStart()
{
#ifdef TRACE
	std::cerr << "PixyImpl::getStart" << std::endl;
#endif

	uint16_t lastw = 0xffff;

	for (;;) {
		int w_int = m_link.getWord();
		uint16_t w = w_int;
		if (w_int == -1) {
			return false;
		}
		else if (lastw == PIXY_START_WORD && w == PIXY_START_WORD) {
			m_blockType = NORMAL_BLOCK;
			break;
		}
		else if (lastw == PIXY_START_WORD && w == PIXY_START_WORD_CC) {
			m_blockType = CC_BLOCK;
			break;
		}
		else if (w == PIXY_START_WORDX) {
			m_link.getByte(); // resync
		}
		lastw = w; 
	}
	return true;
}

////////////////////////////////////////////////////////////////////////

PixyCam::PixyCam( PixySerial& link ): impl(new PixyImpl(link))
{
	;
}

PixyCam::~PixyCam()
{
	delete impl;
}

bool PixyCam::enable()
{
	return impl->enable();
}

bool PixyCam::disable()
{
	return impl->disable();
}

bool PixyCam::isNewData()
{
	return impl->isNewData();
}

std::vector<Block> PixyCam::getBlocks()
{
	return impl->getBlocks();
}

}; // namespace Pixy
