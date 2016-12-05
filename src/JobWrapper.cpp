//
// Created by andrey on 04.12.16.
//

#include <iostream>
#include "JobWrapper.h"
#include "Exceptions.h"


JobWrapper::JobWrapper(const Job &job) : job(job), lastStartTime(UNDEFINED_TIME) {}

bool JobWrapper::isWait(const Time & t) const {
	return (lastStartTime == UNDEFINED_TIME)
           or ((lastStartTime / job.getPeriod()) < (t / job.getPeriod()));
}

int JobWrapper::getPriority(const Time &t) const {
	auto localPeriodTime = t % job.getPeriod();
	if (localPeriodTime < job.getBegin())
		return -1;
	auto priority = job.getEnd() - localPeriodTime - job.getDuration();
	return static_cast<int>(priority);
}

unsigned JobWrapper::getId() const {
	return job.getId();
}

bool JobWrapper::isExpired(const Time &t) const{
	auto curPeriodNumber = t / job.getPeriod();
	if (lastStartTime == UNDEFINED_TIME and curPeriodNumber > 0)
		return true;
	auto lastRunPeriodNumber = lastStartTime / job.getPeriod();
	if (lastRunPeriodNumber < curPeriodNumber) {
		// check move to time over one or more period
		if (curPeriodNumber - lastRunPeriodNumber > 1)
			return true;
		// check possible to run job in new period
		if (not canBeRunInPeriod(t))
			return true;
	}
	//	impossible: lastRunPeriodNumber < curPeriodNumber
	//	if (lastRunPeriodNumber == curPeriodNumber) then job is already run
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

bool JobWrapper::canBeRunInPeriod(const Time &t) const {
	auto localTime = t % job.getPeriod();
	return localTime <= job.getEnd();
	
}

const Job &JobWrapper::getJob() const {
	return job;
}
//
//int JobWrapper::getCriteriaValue(const Time &curTime) const {
//	auto criteriaValue = job.getEnd() - job.getBegin() - job.getDuration();
//	std::cout << job.getId() << " " << criteriaValue << std::endl;
//	if (criteriaValue < 0)
//		throw WRONG_CRITERIA_VALUE;
//	return static_cast<int>(criteriaValue);
//}
