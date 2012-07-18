#ifndef _PACKT_TIMESERVICE_HPP_
#define _PACKT_TIMESERVICE_HPP_

#include "Types.hpp"

#include <time.h>

namespace packt {
    class TimeService {
    public:
        TimeService();

        void reset();
        void update();

        double now();
        float elapsed();

    private:
        float mElapsed;
        double mLastTime;
    };
}
#endif
