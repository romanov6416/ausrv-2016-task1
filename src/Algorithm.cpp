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
		cycles.push_back(Chain());
		auto & curChain = *cycles.rbegin();
		JobWrapper * foundJobPtr;
		while ((foundJobPtr = findAvailableJobs()) != nullptr
		       and curT < border
		       and curChain.size() < maxChainLen) {
			// choose job and push it to chain
			auto & j = popWaitedJobByCriteria(foundJobPtr);
			if (isCycleChanged(j.getDuration()))
				break;
			addJobToChain(j);
			moveTime(j.getDuration());
		}
		// move time to begin of the cycle
		if (curT < border)
			moveTime(cycleDuration - (curT % cycleDuration));
	}
}

void Algorithm::moveTime(const Time &shift) {
	Time newT = curT + shift;
	
	// check the job expired in new time
	for (auto & j : jobs) {
		if (j.isExpired(newT, reserve))
			throw JOB_WORKED_TIME_EXPIRED;
	}
	
	curT = newT;
}


void Algorithm::clear() {
	curT = 0;
	cycles.clear();
}

JobWrapper & Algorithm::popWaitedJobByCriteria(JobWrapper * foundJobPtr) {
	if (foundJobPtr == nullptr)
		throw NULL_POINTER;
	auto jobByCriteriaPtr = foundJobPtr;
	for (auto & j : jobs) {
		if (j.isWait(curT) and j.isCanRun(curT, reserve)) {
			if (jobByCriteriaPtr->getCriteriaValue() > j.getCriteriaValue())
				jobByCriteriaPtr = const_cast<JobWrapper *>(&j);
		}
	}
	return *jobByCriteriaPtr;
}

Algorithm::Algorithm(const std::unordered_set<Job> &jobs, const Time &cycleDuration,
                     const unsigned int maxChainLen, const Percent &reserve) :
	cycleDuration(cycleDuration),
	maxChainLen(maxChainLen),
	reserve(reserve),
    border(1)
{
	for (auto & j : jobs) {
		this->jobs.insert(JobWrapper(j));
		border = boost::math::lcm(border, j.getPeriod());
	}
}

JobWrapper * Algorithm::findAvailableJobs() {
	for (auto & j : jobs)
		if (j.isWait(curT) and j.isCanRun(curT, reserve))
			return const_cast<JobWrapper *>(&j);
	return nullptr;
}

void Algorithm::addJobToChain(JobWrapper &j) {
	cycles.rbegin()->push_back(j);
	j.setLastStartTime(curT);
}

bool Algorithm::isCycleChanged(const Time &shift) {
	return (curT / cycleDuration != (curT + shift) / cycleDuration);
}

