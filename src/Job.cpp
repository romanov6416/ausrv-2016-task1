//
// Created by andrey on 01.12.16.
//
#include <functional>
#include "Job.h"

Job::Job(const unsigned int id, const unsigned int duration, const unsigned int begin, const unsigned int end,
         const unsigned int period) :
	id(id),
	duration(duration),
	begin(begin),
	end(end),
	period(period)
{}

unsigned int Job::getId() const {
	return id;
}

Time Job::getDuration() const {
	return duration;
}

Time Job::getBegin() const {
	return begin;
}

Time Job::getEnd() const {
	return end;
}

Time Job::getPeriod() const {
	return period;
}

bool Job::operator==(const Job &j) const {
	return id == j.id;
}

bool Job::operator!=(const Job &j) const {
	return !(j == *this);
}
