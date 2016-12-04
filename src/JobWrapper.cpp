//
// Created by andrey on 04.12.16.
//

#include "JobWrapper.h"
#include "Exceptions.h"


JobWrapper::JobWrapper(const Job &job) : job(job), lastStartTime(UNDEFINED_TIME) {
	criteriaValue = static_cast<int>(job.getEnd() - job.getBegin() - job.getDuration());
	if (criteriaValue < 0)
		throw WRONG_CRITERIA_VALUE;
}

bool JobWrapper::isWait(const Time & t) const {
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
