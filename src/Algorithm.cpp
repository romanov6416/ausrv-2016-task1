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
			auto &curChain = *cycles.rbegin();
			// update waited jobs
			updateWaitedJobs();
			std::cout << "ew curT " << curT << std::endl;
			
			while (waitedJobs.size() > 0 and curT < border and curChain.size() < maxChainLen) {
				std::cout << "waited jobs size " << waitedJobs.size() << '\n';
				// choose job and push it to chain
				auto j = getWaitedJobByCriteria();
				waitedJobs.erase(j);
				try {
					updateTime(curT + j.getDuration(), false);
				} catch(Exception & e) {
					if (e == CHANGE_CYCLE)
						waitedJobs.insert(j);
						break;
					throw;
				}
				// add job to chain
				curChain.push_back(j);
				std::cout << "waited jobs size " << waitedJobs.size() << '\n';
				std::cout << "curT " << curT << std::endl;
				
			}
//			std::cout << "waitedJobs.size() " << waitedJobs.size() << std::endl;
			// move time to begin of the cycle
			if (curT < border)
				updateTime(curT + cycleDuration - (curT % cycleDuration), true);
		}
	} catch (const char * s) {
		std::cout << "error: " << s << std::endl;
		return;
	}
	std::cout << "success " << std::endl;
}

bool Algorithm::updateTime(const unsigned newT, bool force) {
	// check that "curT <= newT" and out of border
	if (newT < curT)
		throw NEW_TIME_LESS_OLD_TIME;
	
	// check out of range
	if (border < newT and not force)
		throw NEW_TIME_OUT_OF_RANGE;
	
	// change cycle changed
	if ((curT / cycleDuration != newT / cycleDuration) and not force)
		throw CHANGE_CYCLE;
	
	for (auto & j : jobs) {
		unsigned oldPeriodN = curT / j.getPeriod();
		unsigned newPeriodN = newT / j.getPeriod();
//		unsigned periodLocalTime = newT % j.getPeriod();
		// consider reserve in cycle
		unsigned beginWorkedTime = getTotalBeginTime(j, newPeriodN);
		unsigned endWorkedTime = getTotalEndTime(j, newPeriodN);
//		std::cout << oldPeriodN << " " << newPeriodN << std::endl;
		if (newPeriodN - oldPeriodN > 1)
			throw PERIOD_MISSED;
		if (oldPeriodN < newPeriodN) {
			if (waitedJobs.find(j) != waitedJobs.cend())
				throw OLD_PERIOD_IMPOSSIBLE_PERFORM_JOB_IN_OLD_PERIOD;
			if (beginWorkedTime < newT and endWorkedTime < newT)
				// time is out of range of work time for this job
				throw "exception: impossible to perform job in new period";
		} else if (oldPeriodN == newPeriodN) {
			if (waitedJobs.find(j) != waitedJobs.cend() and endWorkedTime < newT)
				throw IMPOSSIBLE_PERFORM_JOB_IN_PERIOD;
		} else
			throw OLD_PERIOD_MORE_NEW_PERIOD;
	}
	curT = newT;
	updateWaitedJobs();
	return false;
}

void Algorithm::updateWaitedJobs() {
	waitedJobs.clear();
		
	for (auto & j : jobs) {
		unsigned periodNumber = curT / j.getPeriod();
		auto totalBegin = getTotalBeginTime(j, periodNumber);
		auto totalEnd = getTotalEndTime(j, periodNumber);
		if (totalBegin <= curT and curT <= totalEnd and isJobWait(j))
			waitedJobs.insert(j);
	}
}

void Algorithm::clear() {
	curT = 0;
	waitedJobs.clear();
	cycles.clear();
}

const Job Algorithm::getWaitedJobByCriteria() {
	auto it = waitedJobs.cbegin();
	for (auto curIt = waitedJobs.cbegin(); curIt != waitedJobs.cend(); ++curIt) {
		auto stockCurIt = (curIt->getEnd() - curIt->getBegin()) - curIt->getDuration();
		auto stockIt = (it->getEnd() - it->getBegin()) - it->getDuration();
		if (stockCurIt < stockIt)
			it = curIt;
	}
	return *it;
}

Algorithm::Algorithm(const std::unordered_set<Job> &jobs, const unsigned int cycleDuration,
                     const unsigned int maxChainLen, const unsigned int reserve) :
	jobs(jobs),
	cycleDuration(cycleDuration),
	maxChainLen(maxChainLen),
	reserve(reserve),
    border(1)
{
	for (auto & j : jobs) {
		border = boost::math::lcm(border, j.getPeriod());
	}
}

bool Algorithm::isJobWait(const Job &job) {
//	auto periodNumber = curT / job.getPeriod();
	auto periodStartTime = curT - (curT % job.getPeriod());
	auto startCycleNumber = periodStartTime / cycleDuration;
	
	if (startCycleNumber * cycleDuration < periodStartTime) {
		// part of cycle is not in job period
		auto totalTime = startCycleNumber * cycleDuration;
		Chain & curCycle = cycles[startCycleNumber];
		unsigned curJobNumber = 0;
		// miss jobs, executed in previous period
		for (; curJobNumber < curCycle.size() and totalTime < periodStartTime; ++curJobNumber)
			totalTime += curCycle[curJobNumber].getDuration();
		// find job in cycle
		for (; curJobNumber < curCycle.size(); ++curJobNumber)
			if (curCycle[curJobNumber] == job)
				return false;
		++startCycleNumber;
	}
	// find job in cycles
	for (auto n = startCycleNumber; n < cycles.size(); ++n)
		for (auto & j : cycles[n])
			if (job == j)
				return false;
	return true;
}

const unsigned Algorithm::getTotalBeginTime(const Job &j, const unsigned periodNumber) {
	auto totalTime = periodNumber * j.getPeriod() + j.getBegin();
	auto cycleLocalTime = totalTime % cycleDuration;
	if (cycleLocalTime < cycleDuration * (100 - reserve) / 100)
		return j.getBegin();
	return j.getPeriod();
}

const unsigned Algorithm::getTotalEndTime(const Job &j, const unsigned periodNumber) {
	auto totalTime = periodNumber * j.getPeriod() + j.getEnd();
	auto cycleLocalTime = totalTime % cycleDuration;
	if (cycleLocalTime < cycleDuration * (100 - reserve) / 100)
		return j.getEnd();
	return cycleDuration * (100 - reserve) / 100;
}


