//
// Created by andrey on 01.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
#define DATA_TRANSMISSION_SHEDULE_ALGORITHM_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "Types.h"
#include "JobWrapper.h"


typedef std::vector<JobWrapper> Chain;


class Algorithm {
public:
	Algorithm(const std::unordered_set<Job> &jobs, const Time &cycleDuration, const unsigned int maxChainLen,
	          const Percent &reserve);
	void compute();
	void printResults();
	void clear();

private:
	void moveTime(const Time &shift);
	bool isCycleChanged(const Time &shift);
	void addJobToChain(Chain & c, JobWrapper &j);
	void check();
	
	JobWrapper * findAvailableJob();
	
	std::unordered_set<JobWrapper> jobs;
	Time usefulCycleDuration;
	Time cycleDuration;
	unsigned maxChainSize;
//	Percent reserve;
	Time border;
	
	Time curT;
	std::vector<Chain> cycles;
};


#endif //DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
