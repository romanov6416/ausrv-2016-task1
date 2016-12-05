//
// Created by andrey on 01.12.16.
//
#include <iostream>

#include "Algorithm.h"
#include "Exceptions.h"


const Time gcd(const Time & a, const Time & b) {
	return b == 0 ? a : gcd(b, a % b);
}

const Time lcm(const Time & a, const Time & b) {
	return abs(a * b) / gcd(a, b);
}


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
			// choose job and push it to chain
			auto & j = *foundJobPtr;
			if (isCycleChanged(j.getDuration()) or isInReserve(j.getDuration()))
				break;
			addJobToChain(curChain, j);
			moveTime(j.getDuration());
		}
		cycles.push_back(curChain);
		
		// move to time of the beginning of the next cycle
		moveTime(cycleDuration - (curT % cycleDuration));
		
	}
	
//	Time t;
//	unsigned i = 0;
//	std::cout << "Algo (cycle max == " << maxChainSize << ")" << std::endl;
//	for (; i < cycles.size(); ++i) {
//		t = i * cycleDuration;
//		auto & chain = cycles[i];
//		std::cout << "chain " << i << std::endl;
//		unsigned j = 0;
//		for (; j < chain.size(); ++j) {
//			auto & job = chain[j];
//			std::cout << job.getId() << " [" << t;
//			t += job.getDuration();
//			std::cout << ", " << t << "]" << std::endl;
//		}
//		std::cout << "end of chain (len == " << j << ")" << std::endl;
//	}
	
}

/*

const Time lcm(const Time & l, const Time & r) {
	auto a = l, b = r;
	return
}
*/

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
		border = lcm(border, j.getPeriod());
//		border = boost::math::lcm(border, j.getPeriod());
	}
}

JobWrapper * Algorithm::findAvailableJob() {
	const JobWrapper * foundJobPtr = nullptr;
	int foundPriority = -1;
	for (auto it = jobs.begin(); it != jobs.end(); ++it) {
		auto & job = *it;
		auto priority = job.getPriority(curT);
		if (priority >= 0 and job.isWait(curT)) {
			if ((foundPriority < 0) or (priority < foundPriority)) {
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

void Algorithm::check() {
	for (auto & checkedJobWrap : jobs) {
		auto & checkedJob = checkedJobWrap.getJob();
		std::vector<bool> jobCalled;
		for (unsigned i = 0; i < (border / checkedJob.getPeriod()); ++i)
			jobCalled.push_back(false);
		
		Time time;
		for (unsigned i = 0; i < cycles.size(); ++i) {
			auto & chain = cycles[i];
			time = i * cycleDuration;
			for (unsigned j = 0; j < chain.size(); ++j) {
				auto & job = chain[j].getJob();
				auto localPeriodTime = time % checkedJob.getPeriod();
				if (job.getId() == checkedJobWrap.getId()) {
					// check time in [begin,end] interval
					if (not (job.getBegin() <= localPeriodTime
					         and localPeriodTime + job.getDuration() <= job.getEnd()))
						throw;
					jobCalled[time / job.getPeriod()] = true;
				}
				time += job.getDuration();
				// check time not in reserve time
				if (time % cycleDuration > usefulCycleDuration)
					throw;
			}
		}
		// check that job is processed in every period
		for (auto called : jobCalled)
			if (not called)
				throw;
	}
}

void Algorithm::printResults() {
	std::cout << "r_rf = " << (static_cast<float>(cycleDuration) - usefulCycleDuration) / cycleDuration
	          << std::endl;
	std::cout << "r_mcc = " << maxChainSize << std::endl;
	
	for (unsigned i = 0; i < cycles.size(); ++i) {
		std::cout << i * cycleDuration;
		for (unsigned j = 0; j < cycles[i].size(); ++j) {
			std::cout << ' ' << cycles[i][j].getId();
		}
		std::cout << std::endl;
	}
}

bool Algorithm::isInReserve(const Time & shift) {
	return (curT + shift) % cycleDuration > usefulCycleDuration;
}
