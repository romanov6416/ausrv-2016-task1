//
// Created by andrey on 02.12.16.
//

#ifndef DATA_TRANSMISSION_SHEDULE_EXCEPTIONS_H
#define DATA_TRANSMISSION_SHEDULE_EXCEPTIONS_H

enum Exception {
	OPEN_FILE_ERROR,
	
	// set time errors
	NEW_TIME_LESS_OLD_TIME,
	NEW_TIME_OUT_OF_RANGE,
	CHANGE_CYCLE,
	PERIOD_MISSED,
	OLD_PERIOD_IMPOSSIBLE_PERFORM_JOB_IN_OLD_PERIOD,
	OLD_PERIOD_MORE_NEW_PERIOD,
	IMPOSSIBLE_PERFORM_JOB_IN_PERIOD,
	
};
#endif //DATA_TRANSMISSION_SHEDULE_EXCEPTIONS_H