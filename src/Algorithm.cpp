//
// Created by andrey on 01.12.16.
//
#include "Algorithm.h"


#include <iostream>
#include <boost/math/common_factor_rt.hpp>

#include "Exceptions.h"

void Algorithm::compute() {
	clear();
	try {
		while (curT < border) {
			// init empty chain
			cycles.push_back(Chain());
			auto & curChain = *cycles.rbegin();
//			std::cout << "hear" << std::endl;
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
//			std::cout << "ew curT " << curT << std::endl;
			
		}
	} catch (const char * s) {
//		std::cout << "error: " << s << std::endl;
		return;
	}
//	std::cout << "success " << std::endl;
}

void Algorithm::moveTime(const Time &shift) {
	Time newT = curT + shift;
	
//	// check out of range
//	if (border < newT)
//		throw TIME_OUT_OF_RANGE;
	
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

Algorithm::Algorithm(const std::unordered_set<Job> &jobs, const unsigned int cycleDuration,
                     const unsigned int maxChainLen, const unsigned int reserve) :
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


JobWrapper::JobWrapper(const Job &job) : job(job), lastStartTime(UNDEFINED_TIME) {
	criteriaValue = static_cast<int>(job.getEnd() - job.getBegin() - job.getDuration());
	if (criteriaValue < 0)
		throw WRONG_CRITERIA_VALUE;
}

bool JobWrapper::isWait(const Time & t) const {
//	std::cout << lastStartTime << std::endl;
	return lastStartTime == UNDEFINED_TIME
	       or (lastStartTime / job.getPeriod()) < (t / job.getPeriod());
}

bool JobWrapper::isCanRun(const Time &t, const Percent & reserve) const {
	auto localPeriodTime = t % job.getPeriod();
	// note reserve; end of run time can be changed
	auto endRunTime = std::min(job.getEnd(), job.getPeriod() * (MAX_PERCENT - reserve) / MAX_PERCENT);
	return (localPeriodTime < job.getBegin()) // can not be running now, but can be running later
	       or (localPeriodTime + job.getDuration() <= endRunTime); // check we have time to run job
	
}

unsigned JobWrapper::getId() const {
	return job.getId();
}

bool JobWrapper::isExpired(const Time &t, const Percent &reserve) const{
	auto lastPeriodNumber = lastStartTime / job.getPeriod();
	auto curPeriodNumber = t / job.getPeriod();
	if (lastPeriodNumber < curPeriodNumber) {
		if (curPeriodNumber - lastPeriodNumber > 1)
			return true;
		if (not isCanRun(t, reserve))
			return true;
	}
	return false;
}

const Time JobWrapper::getDuration() const {
	return job.getDuration();
}

bool JobWrapper::operator==(const JobWrapper &rhs) const {
	return job == rhs.job;
}

bool JobWrapper::operator!=(const JobWrapper &rhs) const {
	return !(rhs == *this);
}

void JobWrapper::setLastStartTime(Time lastStartTime) {
	this->lastStartTime = lastStartTime;
}

int JobWrapper::getCriteriaValue() const {
	return criteriaValue;
}
