//
// Created by andrey on 01.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_JOB_H
#define DATA_TRANSMISSION_SHEDULE_JOB_H


#include <cstdlib>
#include <functional>

class Job {
public:
	Job(const unsigned int id, const unsigned int duration, const unsigned int begin, const unsigned int end,
	    const unsigned int period);
	
	unsigned int getId() const;
	
	unsigned int getDuration() const;
	
	unsigned int getBegin() const;
	
	unsigned int getEnd() const;
	
	bool operator==(const Job &j) const;
	
	bool operator!=(const Job &rhs) const;
	
	unsigned int getPeriod() const;
	
private:
	unsigned id;
	unsigned duration;
	unsigned begin;
	unsigned end;
	unsigned period;
	
	
};


namespace std {
	template <> struct hash<Job>
	{
		size_t operator()(const Job & j) const
		{
			return std::hash<int>()(j.getId());
		}
	};
}

#endif //DATA_TRANSMISSION_SHEDULE_JOB_H
