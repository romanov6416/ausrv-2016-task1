//
// Created by andrey on 02.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_PARSER_H
#define DATA_TRANSMISSION_SHEDULE_PARSER_H


#include <vector>

#include "Job.h"

class Parser {
	static const int getInt(std::ifstream & f);
public:
	static std::vector<Job> parseJobs(const char * filename);
};


#endif //DATA_TRANSMISSION_SHEDULE_PARSER_H
