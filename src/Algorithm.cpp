//
// Created by andrey on 01.12.16.
//
#include "Algorithm.h"


#include <iostream>
#include <boost/math/common_factor_rt.hpp>

#include "Exceptions.h"

void Algorithm::compute() {
	clear();
	while (curT < border) {
		// init empty chain
		auto curChain = Chain();
		
		// find job to push to chain and check cycle changed if job would be completed
		JobWrapper * foundJobPtr;
		while ((foundJobPtr = findAvailableJob()) != nullptr
		       and curT < border
		       and curChain.size() < maxChainLen) {
//			std::cout << "hear" << std::endl;
			// choose job and push it to chain
			auto & j = *foundJobPtr;
			if (isCycleChanged(j.getDuration()))
				break;
			addJobToChain(curChain, j);
			moveTime(j.getDuration());
			
//			std::cout << j.getId() << " [" << curT - j.getDuration();
//			std::cout << ", " << curT << "]" << std::endl;
//			std::cout << "last call" << j.
		}
		cycles.push_back(curChain);
		
		// move time to begin of the cycle
		moveTime(cycleDuration - (curT % cycleDuration));
		
	}
	
	Time t = 0;
	unsigned i;
//	if (reserve == 19 and maxChainLen == 51)
//		reserve = 17;
	i = 0;
	std::cout << "Algo (cycle max == " << maxChainLen << ")" << std::endl;
	for (; i < cycles.size(); ++i) {
		t = i * cycleDuration;
		auto & chain = cycles[i];
		std::cout << "chain " << i << std::endl;
		unsigned j = 0;
		for (; j < chain.size(); ++j) {
			auto & job = chain[j];
			std::cout << job.getId() << " [" << t;
			t += job.getDuration();
			std::cout << ", " << t << "]" << std::endl;
		}
		std::cout << "end of chain (len == " << j << ")" << std::endl;
	}
//	if (reserve == 17 and maxChainLen == 51)
//		reserve = 17;
}

void Algorithm::moveTime(const Time &shift) {
	Time newT = curT + shift;
	
	// check the job expired in new time
	for (auto & j : jobs) {
		if (j.isExpired(newT))
			throw JOB_WORKED_TIME_EXPIRED;
	}
	
	curT = newT;
}


void Algorithm::clear() {
	curT = 0;
	cycles.clear();
}

Algorithm::Algorithm(const std::unordered_set<Job> &jobs, const Time &cycleDuration,
                     const unsigned int maxChainLen, const Percent &reserve) :
	cycleDuration(cycleDuration * (MAX_PERCENT - reserve) / MAX_PERCENT),
	maxChainLen(maxChainLen),
//	reserve(reserve),
    border(1)
{
	for (auto & j : jobs) {
		this->jobs.insert(JobWrapper(j));
		border = boost::math::lcm(border, j.getPeriod());
	}
}

JobWrapper * Algorithm::findAvailableJob() {
	const JobWrapper * foundJobPtr = nullptr;
	int foundPriority = -1;
	for (auto it = jobs.begin(); it != jobs.end(); ++it) {
		auto & job = *it;
		auto priority = job.getPriority(curT);
		if (priority >= 0 and job.isWait(curT)) {
//			if (job.getId() == 0)
//				std::cout << job.getId() << " and " << job.getPriority(curT) << std::endl;
//			if (job.getId() == 156)
//				std::cout << job.getId() << " and " << job.getPriority(curT) << std::endl;
			if (foundPriority < 0 or priority < foundPriority) {
//				foundJobPtr = const_cast<JobWrapper *>(&job);
				foundPriority = priority;
				foundJobPtr = &job;
			}
		}
	}
	return const_cast<JobWrapper *>(foundJobPtr);
}

void Algorithm::addJobToChain(Chain & c, JobWrapper &j) {
	c.push_back(j);
	j.setLastStartTime(curT);
}

bool Algorithm::isCycleChanged(const Time &shift) {
	return (curT / cycleDuration) != ((curT + shift) / cycleDuration);
}
//
//bool Algorithm::isInReserved(const Time &shift) {
//	return curT % cycleDuration + shift > cycleDuration;
//}

