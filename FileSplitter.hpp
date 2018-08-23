#ifndef FILESPLITTER_HPP_INCLUDED
#define FILESPLITTER_HPP_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>

#include "sha256.h"
#include "../utils/utils.hpp"

namespace GNB
{
	namespace FileSplitter
	{
		struct FileSegmentHeader
		{
			char previousSegmentSHA256[64];
		};

		bool Split(std::string filename, size_t nb_segments);
		bool Join(std::string filename, std::vector<std::string> partFiles);
	}
}

#endif // FILESPLITTER_HPP_INCLUDED
