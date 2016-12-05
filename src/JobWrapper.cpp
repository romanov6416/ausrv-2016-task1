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
	auto lastPeriodNumber = lastStartTime / job.getPeriod();
	auto curPeriodNumber = t / job.getPeriod();
	if (lastPeriodNumber < curPeriodNumber) {
		if (curPeriodNumber - lastPeriodNumber > 1)
			return true;
		if (getPriority(t) < 0)
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
//
//int JobWrapper::getCriteriaValue(const Time &curTime) const {
//	auto criteriaValue = job.getEnd() - job.getBegin() - job.getDuration();
//	std::cout << job.getId() << " " << criteriaValue << std::endl;
//	if (criteriaValue < 0)
//		throw WRONG_CRITERIA_VALUE;
//	return static_cast<int>(criteriaValue);
//}
