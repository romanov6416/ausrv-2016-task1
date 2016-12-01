//
// Created by andrey on 01.12.16.
//

#include <iostream>
#include "Algorithm.h"

void Algorithm::compute() {
	try {
		while (curT < border) {
			// init empty chain
			cycles.push_back(Chain());
			auto &curChain = *cycles.rbegin();
			// update waited jobs
			updateWaitedJobs();
			
			while (waitedJobs.size() > 0 and curT < border) {
				auto j = getWaitedJobByCriteria();
				curChain.push_back(j);
				updateTime(curT + j.getDuration(), false);
			}
			if (curT < border)
				updateTime((curT / cycleDuration + 1) * cycleDuration, true);
		}
	} catch (const char * s) {
		std::cout << "error: " << s << std::endl;
		return;
	}
	std::cout << "success " << std::endl;
}

bool Algorithm::updateTime(const unsigned newT, bool ignoreCycle = false) {
	if (newT < curT or border < newT)
		throw "exception";
	
	if ((curT / cycleDuration != newT / cycleDuration) and not ignoreCycle)
		throw "exception";
	
	for (auto & j : jobs) {
		unsigned oldPeriod = (curT + 1) / j.getPeriod();
		unsigned newPeriod = (newT + 1) / j.getPeriod();
		
		if (newPeriod - oldPeriod > 1)
			throw "exception: one or more period will be missed";
		if (oldPeriod < newPeriod) {
			if (waitedJobs.find(j) == waitedJobs.cend())
				throw "exception: impossible to perform job in current period";
			if (j.getEnd() < newT % j.getPeriod())
				throw "exception: impossible to perform job in new period";
		} else if (oldPeriod == newPeriod) {
			if (waitedJobs.find(j) != waitedJobs.cend() and j.getEnd() < newT % j.getPeriod())
				throw "exception: impossible to perform job in current period";
		} else
			throw "exception";
	}
	curT = newT;
	updateWaitedJobs();
	return false;
}

void Algorithm::updateWaitedJobs() {
	waitedJobs.clear();
	for (auto & j : jobs) {
		unsigned localTime = curT / j.getPeriod();
		if (j.getBegin() < localTime and localTime <= j.getEnd())
			waitedJobs.insert(j);
	}
}

void Algorithm::clear() {
	curT = 0;
	waitedJobs.clear();
	cycles.clear();
}

const Job & Algorithm::getWaitedJobByCriteria() {
	auto it = waitedJobs.cbegin();
	for (auto curIt = waitedJobs.cbegin(); curIt != waitedJobs.cend(); ++curIt) {
		auto stockCurIt = (curIt->getEnd() - curIt->getBegin()) - curIt->getDuration();
		auto stockIt = (it->getEnd() - it->getBegin()) - it->getDuration();
		if (stockCurIt < stockIt)
			it = curIt;
	}
	return *it;
}


