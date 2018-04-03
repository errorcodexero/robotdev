#pragma once
#include "message_dest_file.h"
#include <fstream>
#include <string>

class messageDestSequentialFile : public messageDestFile {
public:
	/// \brief Sequential file naming with a given prefix and extension
	messageDestSequentialFile(const std::string& prefix, const std::string& ext) {
		std::ifstream f;
		unsigned long int i = 0;
		std::string filename;
		bool opened;
		do {
			filename = prefix + std::to_string(i) + ext;
			i++;
			f.open(filename);
			opened = f.is_open();
			f.close();
		} while (opened);
		setFile(filename);
	}
};