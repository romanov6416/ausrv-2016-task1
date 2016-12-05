//
// Created by andrey on 04.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_JOBWRAPPER_H
#define DATA_TRANSMISSION_SHEDULE_JOBWRAPPER_H

#include "Types.h"
#include "Job.h"

class JobWrapper {
	const Job & job;
	Time lastStartTime;
	std::vector<Time> called;
public:
	JobWrapper(const Job &job);
	unsigned getId() const;
	const Time getDuration() const;
	bool isWait(const Time & t) const;

	void setLastStartTime(Time lastStartTime);
//	int getCriteriaValue(const Time &curTime) const;
	const Job &getJob() const;
	
	bool operator==(const JobWrapper &rhs) const;
	bool operator!=(const JobWrapper &rhs) const;
	int getPriority(const Time &t) const; // return run priority if can be run or value<0
	bool canBeRunInPeriod(const Time &t) const;
	bool isExpired(const Time &t) const ;
};

namespace std {
	template <> struct hash<JobWrapper>
	{
		size_t operator()(const JobWrapper & jWrap) const
		{
			return std::hash<int>()(jWrap.getId());
		}
	};
}


#endif //DATA_TRANSMISSION_SHEDULE_JOBWRAPPER_H
