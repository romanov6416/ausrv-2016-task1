#include <iostream>

#include "Parser.h"
#include "Algorithm.h"

int main(int argc, char * argv[]) {
	std::cout << "Hello, World!" << std::endl;
	try {
		auto jobsVector(Parser::parseJobs("../data/S1_SHIFT_20ms_025_055_021.txt"));
		std::unordered_set jobs(jobsVector);
//		Algorithm a(jobs, );
	} catch(const char * err) {
		std::cerr << err << std::endl;
	} catch (...) {
		std::cerr << "someone error" << std::endl;
	}
	
	return 0;
}