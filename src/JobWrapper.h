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
	int criteriaValue;
public:
	JobWrapper(const Job &job);
	unsigned getId() const;
	const Time getDuration() const;
	bool isWait(const Time & t) const;

	void setLastStartTime(Time lastStartTime);
	int getCriteriaValue() const;

	bool operator==(const JobWrapper &rhs) const;
	bool operator!=(const JobWrapper &rhs) const;
	bool isCanRun(const Time & t, const Percent & ) const;
	bool isExpired(const Time &t, const Percent &) const ;
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
