//
// Created by andrey on 01.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
#define DATA_TRANSMISSION_SHEDULE_ALGORITHM_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
//#include <hash>
#include "Job.h"

typedef std::vector<Job> Chain;

class Algorithm {
	std::unordered_set<Job> jobs;
	const unsigned cycleDuration;
	const unsigned maxChainLen;
	const unsigned reserve; // percents
	const unsigned border;
	
	unsigned curT;
	std::unordered_set<Job> waitedJobs;
	std::vector<Chain> cycles;
	
	bool updateTime(const unsigned newT, bool ignoreCycle = false);
	void updateWaitedJobs();
	const Job & getWaitedJobByCriteria();
	
//	void update()
public:
	void compute();
	void clear();
};


#endif //DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
