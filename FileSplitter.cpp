#include "FileSplitter.hpp"

bool GNB::FileSplitter::Split(std::string filename, size_t nb_segments)
{
	SHA256 sha256;
	FileSegmentHeader headerFileSegment;
	std::strcpy(headerFileSegment.previousSegmentSHA256, sha256("").c_str());

	std::fstream fFileSource(filename.c_str(), std::fstream::in | std::fstream::binary | std::fstream::ate);
	if(fFileSource.is_open())
	{
		size_t sizeFileSource(fFileSource.tellg());
		size_t sizeFileSegment = sizeFileSource/nb_segments;

		fFileSource.seekg(0, fFileSource.beg);
		for(size_t i=0; i<nb_segments; i++)
		{
			if(i == nb_segments-1)
				sizeFileSegment += sizeFileSource%nb_segments;

			std::string fileSegmentPath = filename+"__"+intToString(i);
			std::fstream fFileSegment(fileSegmentPath.c_str(), std::fstream::out | std::fstream::binary | std::fstream::trunc);
			if(fFileSegment.is_open())
			{
				fFileSegment.write(reinterpret_cast<char*>(&headerFileSegment), sizeof(FileSegmentHeader));
				for(size_t o=0; o<sizeFileSegment; o++)
				{
					char c = fFileSource.get();
					fFileSegment.put(c);
					sha256.add(&c, 1);
				}
				std::strcpy(headerFileSegment.previousSegmentSHA256, sha256.getHash().c_str());
				sha256.reset();

				fFileSegment.close();
			}
			else
				return false;
		}

		fFileSource.close();
		return true;
	}
	else
		return false;
}


bool GNB::FileSplitter::Join(std::string filename, std::vector<std::string> partFiles)
{
	SHA256 sha256;
	FileSegmentHeader headerFileSegment;
	std::map<std::string, std::string> fileSegmentPreviousMap; // previous part, current file
	std::string previous_fileSegmentHash("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

	// Reading part files header
	for(size_t i=0; i<partFiles.size(); i++)
	{
		std::fstream fFileSegment(partFiles[i].c_str(), std::fstream::in | std::fstream::binary);
		if(fFileSegment.is_open())
		{
            fFileSegment.read(reinterpret_cast<char*>(&headerFileSegment), sizeof(FileSegmentHeader));
			fileSegmentPreviousMap.insert(std::make_pair(headerFileSegment.previousSegmentSHA256, partFiles[i]));

            fFileSegment.close();
		}
		else
			return false; // Open error
	}

	std::fstream fFileToRebuild(filename.c_str(), std::fstream::out | std::fstream::binary | std::fstream::trunc);
	if(fFileToRebuild.is_open())
	{
		for(size_t i=0; i<fileSegmentPreviousMap.size(); i++)
		{
			if(fileSegmentPreviousMap.count(previous_fileSegmentHash) == 1)
			{
				std::fstream fFileSegment(fileSegmentPreviousMap.at(previous_fileSegmentHash).c_str(), std::fstream::in | std::fstream::binary | std::fstream::ate);
				if(fFileSegment.is_open())
				{
					size_t fileSegmentSize = fFileSegment.tellg()-sizeof(FileSegmentHeader);
					fFileSegment.seekg(sizeof(FileSegmentHeader), std::fstream::beg);
					for(size_t o=0; o<fileSegmentSize; o++)
					{
						char c = fFileSegment.get();
						fFileToRebuild.put(c);
						sha256.add(&c, 1);
					}

					previous_fileSegmentHash = sha256.getHash();
					sha256.reset();

					fFileSegment.close();
				}
				else
					return false;
			}
			else
				return false;
		}
		fFileToRebuild.close();

		return true;
	}
	else
		return false; // Open error
}
