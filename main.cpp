#include <iostream>
#include <vector>
#include "FileSplitter.hpp"

using namespace std;

int main()
{
	std::vector<std::string> partFiles;
		partFiles.push_back("test.avi__0");
		partFiles.push_back("test.avi__1");
		partFiles.push_back("test.avi__2");
		partFiles.push_back("test.avi__3");
	std::cout << "Split: " << GNB::FileSplitter::Split("test.avi", 4) << std::endl;
	std::cout << "Join: " << GNB::FileSplitter::Join("testRebuild.avi", partFiles) << std::endl;

    return 0;
}
