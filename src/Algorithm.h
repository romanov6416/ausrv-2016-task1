//
// Created by andrey on 01.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
#define DATA_TRANSMISSION_SHEDULE_ALGORITHM_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
//#include <math>
//#include <boost>
//#include <hash>
#include "Job.h"

typedef std::vector<Job> Chain;

class Algorithm {
public:
	Algorithm(const std::unordered_set<Job> &jobs, const unsigned int cycleDuration, const unsigned int maxChainLen,
	          const unsigned int reserve);
	void compute();
	void clear();

private:
	std::unordered_set<Job> jobs;
	unsigned cycleDuration;
	unsigned maxChainLen;
	unsigned reserve; // percents
	unsigned border;
	
	unsigned curT;
	std::unordered_set<Job> waitedJobs;
	std::vector<Chain> cycles;
	
	bool updateTime(const unsigned newT, bool ignoreCycle);
	void updateWaitedJobs();
	const Job & getWaitedJobByCriteria();
	
//	void update()
};


#endif //DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
