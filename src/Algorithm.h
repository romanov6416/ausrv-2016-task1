//
// Created by andrey on 01.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
#define DATA_TRANSMISSION_SHEDULE_ALGORITHM_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
//#include <math>
//#include <boost>
//#include <hash>
#include "Job.h"

#include "Types.h"



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


typedef std::vector<JobWrapper> Chain;



class Algorithm {
public:
	Algorithm(const std::unordered_set<Job> &jobs, const unsigned int cycleDuration, const unsigned int maxChainLen,
	          const unsigned int reserve);
	void compute();
	void clear();

private:
	void moveTime(const Time &shift);
	bool isCycleChanged(const Time &shift);
	void addJobToChain(JobWrapper &j);
	JobWrapper & popWaitedJobByCriteria(JobWrapper *foundJobPtr);
	
	JobWrapper * findAvailableJobs();
	
	std::unordered_set<JobWrapper> jobs;
	Time cycleDuration;
	unsigned maxChainLen;
	Percent reserve; // percents
	Time border;
	
	Time curT;
//	std::unordered_set<Job> waitedJobs;
	std::vector<Chain> cycles;
//	void update()
};


#endif //DATA_TRANSMISSION_SHEDULE_ALGORITHM_H
