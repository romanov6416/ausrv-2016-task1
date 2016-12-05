#include <iostream>

#include "Parser.h"
#include "Algorithm.h"
#include "Exceptions.h"

int main(int argc, char * argv[]) {
	if (argc != 3) {
		std::cerr << "usage: ./prog_name <jobs_definition_filepath> <cycle_duration>" << std::endl;
		return -1;
	}
	try {
		//"../data/S1_SHIFT_20ms_025_055_021.txt"
		auto jobs(Parser::parseJobs(argv[1]));
		
		std::string::size_type sz;
		int cycleDuration = std::stoi(argv[2], &sz, 10);
		if (sz < std::string(argv[2]).size() or cycleDuration <= 0) {
			std::cerr << "usage: ./prog_name <jobs_definition_filepath> <cycle_duration>" << std::endl;
			return -2;
		}
		cycleDuration *= 1000; // convert to us
		
		Algorithm * solve = nullptr;
		Percent bestReserve = UNDEFINED_PERCENT;
		for (Percent reserve = MAX_PERCENT - 1; reserve >= MIN_PERCENT; --reserve) {
			for (unsigned maxChainSize = 1; maxChainSize < jobs.size(); ++maxChainSize) {
				try {
					Algorithm a(jobs, static_cast<unsigned>(cycleDuration), static_cast<int>(maxChainSize), reserve);
					a.compute();
					solve = new Algorithm(a);
				} catch (Exception &e) {
//					std::cout << "fail to build plan with reserve " << reserve << "% (code " <<  e << ")" << std::endl;
					continue;
				}
				//			std::cout << "success to build plat with reserve " << reserve << "% (code " <<  e << ")" << std::endl;
				bestReserve = reserve;
//				minChainMaxSize = maxChainSize;
				break;
			}
			if (bestReserve == UNDEFINED_PERCENT) {
//				std::cout << "for reserve " << reserve << "% solving is not found" << std::endl;
				continue;
			}
			break;
		}
		if (solve == nullptr) {
			std::cout << "the schedule can not be built" << std::endl;
		} else {
			solve->printResults();
		}
	} catch(const char * err) {
		std::cerr << "string exc: " << err << std::endl;
	} catch(Exception e) {
		std::cerr << "exception: " << e << std::endl;
	} catch (...) {
		std::cerr << "someone error" << std::endl;
	}
	
	return 0;
}