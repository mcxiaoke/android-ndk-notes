#include "TimeService.hpp"
#include "Log.hpp"

#include <cstdlib>

namespace packt {
    TimeService::TimeService() :
        mElapsed(0.0f),
        mLastTime(0.0f) {
        srand(time(NULL));
    }

    void TimeService::reset() {
        Log::info("Resetting TimeService.");
        mElapsed = 0.0f;
        mLastTime = now();
    }

    void TimeService::update() {
        // Checks elapsed time since last frame. It is important to
        // work on double with current time to avoid losing accuracy
        // Then we can go back to float for elapsed time.
        double lCurrentTime = now();
        mElapsed = (lCurrentTime - mLastTime);
        mLastTime = lCurrentTime;
    }

    double TimeService::now() {
        // Retrieves current time with a monotonic clock to ensure
        // time goes forward and is not subject to system changes.
        timespec lTimeVal;
        clock_gettime(CLOCK_MONOTONIC, &lTimeVal);
        return lTimeVal.tv_sec + (lTimeVal.tv_nsec * 1.0e-9);
    }

    float TimeService::elapsed() {
        return mElapsed;
    }
}
