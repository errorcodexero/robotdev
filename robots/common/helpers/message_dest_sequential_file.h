#pragma once
#include "message_dest_file.h"
#include <fstream>
#include <string>
#include "unistd.h"
#include <sys/types.h>
#include <sys/stat.h>

class messageDestSequentialFile : public messageDestFile {
public:
	/// \brief Sequential file naming with a given prefix and extension
	messageDestSequentialFile(const std::string& prefix, const std::string& ext) {
		unsigned long int i = 0;
		std::string filename;
		bool opened;
		do {
			filename = prefix + std::to_string(i) + ext;
			i++;
			struct stat buffer;
			opened = (stat(filename.c_str(), &buffer)==0);
		} while (opened);
		setFile(filename);
	}
};