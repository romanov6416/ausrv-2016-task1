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
		       and curChain.size() < maxChainSize) {
//			std::cout << "hear" << std::endl;
			// choose job and push it to chain
			auto & j = *foundJobPtr;
			if (isCycleChanged(j.getDuration()))
				break;
			addJobToChain(curChain, j);
			moveTime(j.getDuration());
			
//			std::cout << j.getId() << " [" << curT - j.getDuration();
//			std::cout << ", " << curT << "]" << std::endl;
//			std::cout << "job ptr " << &j << std::endl;
		}
		cycles.push_back(curChain);
		
		// move to time of the beginning of the next cycle
		moveTime(cycleDuration - (curT % cycleDuration));
		
	}
	check();
}

void Algorithm::moveTime(const Time &shift) {
	Time newT = curT + shift;
	
	// check the job expiring when time would be moved
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
	usefulCycleDuration(cycleDuration * (MAX_PERCENT - reserve) / MAX_PERCENT),
	cycleDuration(cycleDuration),
	maxChainSize(maxChainLen),
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
//			if (job.getId() == 14)
//				std::cout << job.getId() << " and " << job.getPriority(curT) << std::endl;
			if ((foundPriority < 0) or (priority < foundPriority)) {
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
	return (curT / usefulCycleDuration) != ((curT + shift) / usefulCycleDuration);
}

// validate answer
void Algorithm::check() {
	for (auto & jobWrap : jobs) {
		auto & job = jobWrap.getJob();
		std::vector<bool> jobCalled;
		for (unsigned i = 0; i < (border / job.getPeriod()); ++i)
			jobCalled.push_back(false);
		
		Time time;
		for (unsigned i = 0; i < cycles.size(); ++i) {
			auto & chain = cycles[i];
			time = i * cycleDuration;
			for (unsigned j = 0; j < chain.size(); ++j) {
				auto localPeriodTime = time % job.getPeriod();
				if (chain[j].getId() == jobWrap.getId()) {
					// check time in [begin,end] interval
					if (not (job.getBegin() <= localPeriodTime
					         and localPeriodTime + job.getDuration() <= job.getEnd()))
						throw;
					// check time not in reserve time
					if (time % cycleDuration > usefulCycleDuration)
						throw;
					jobCalled[time / job.getPeriod()] = true;
				}
				time += job.getDuration();
			}
		}
		// check that job is processed in every period
		for (auto called : jobCalled)
			if (not called)
				throw;
	}
}

void Algorithm::printResults() {
	std::cout << "r_rf = " << static_cast<float>(usefulCycleDuration) / cycleDuration << std::endl;
	std::cout << "r_mcc = " << maxChainSize << std::endl;
	
//	Time t = 0;
	for (unsigned i = 0; i < cycles.size(); ++i) {
		std::cout << i * cycleDuration;
		for (unsigned j = 0; j < cycles[i].size(); ++j) {
			std::cout << ' ' << cycles[i][j].getId();
		}
		std::cout << std::endl;
	}
}
