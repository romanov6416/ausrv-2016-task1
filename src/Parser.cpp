//
// Created by andrey on 02.12.16.
//

#include <fstream>
#include <iostream>
#include <limits>
#include <unordered_set>
#include "Job.h"
#include "Parser.h"
#include "Exceptions.h"

std::unordered_set<Job> Parser::parseJobs(const char *filename) {
	std::ifstream f(filename, std::ios::in);
	if (not f.is_open())
		throw OPEN_FILE_ERROR;
	std::unordered_set<Job> jobs;
	for (int n = getInt(f); n >= 0; n = getInt(f)) {
		unsigned id = n;
		
		if ((n = getInt(f)) < 0)
			throw "bad file";
		unsigned nWords = n;
		
		if ((n = getInt(f)) <= 0)
			throw "bad file";
		unsigned frequency = n;
		
		if ((n = getInt(f)) < 0)
			throw "bad file";
		unsigned beginShift = n;
		
		if ((n = getInt(f)) < 0)
			throw "bad file";
		unsigned endShift = n;
		
		unsigned duration = nWords * 20;
		unsigned period = 1000000 / frequency; // convert to us period
		beginShift *= 1000; // convert to us
		endShift = (endShift == 0) ? period : endShift * 1000; // convert to us
		
		Job j(id, duration, beginShift, endShift, period);
		jobs.insert(j);
	}
	return jobs;
}

const int Parser::getInt(std::ifstream &f) {
	std::string s;
	f >> s;
	if (s.size() > 0 and s[0] == '#') {
		f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return getInt(f);
	}
	if (s.size() <= 0)
		return -1;
	std::string::size_type sz;
	int i = std::stoi(s.c_str(), &sz, 10);
	if (sz < s.size())
		return -2;
	return i;
}
