/*
 * TimeService.cpp
 *
 *  Created on: 2012-7-11
 *      Author: mcxiaoke
 */

#include "TimeService.hpp"
#include "Log.hpp"

namespace demo {
TimeService::TimeService() :
        mElapsed(0.0f), mLastTime(0.0f) {
}

void TimeService::reset() {
    Log::info("Resetting TimeService.");
    mElapsed = 0;
    mLastTime = now();
}

void TimeService::update() {
    double curTime = now();
    mElapsed = curTime - mLastTime;
    mLastTime = curTime;
}

double TimeService::now() {
    timespec timeVal;
    clock_gettime(CLOCK_MONOTONIC, &timeVal);
    return timeVal.tv_sec +(timeVal.tv_nsec*1.0e-9);
//    gettimeofday(&timeVal, NULL);
//    return (timeVal.tv_sec * 1000.0) + (timeVal.tv_usec / 1000.0);
}

float TimeService::elapsed() {
    return mElapsed;
}

}

