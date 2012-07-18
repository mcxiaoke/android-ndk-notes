#include "Sensor.hpp"
#include "EventLoop.hpp"
#include "Log.hpp"

namespace packt {
    Sensor::Sensor(EventLoop& pEventLoop, int32_t pSensorType):
        mEventLoop(pEventLoop),
        mSensor(NULL),
        mSensorType(pSensorType) {
    }

    status Sensor::toggle() {
        return (mSensor != NULL) ? disable() : enable();
    }

    status Sensor::enable() {
        if (mEventLoop.mEnabled) {
            mSensor = ASensorManager_getDefaultSensor(
                mEventLoop.mSensorManager, mSensorType);
            if (mSensor != NULL) {
                if (ASensorEventQueue_enableSensor(
                    mEventLoop.mSensorEventQueue, mSensor) < 0) {
                    goto ERROR;
                }

                const char* lName = ASensor_getName(mSensor);
                const char* lVendor = ASensor_getVendor(mSensor);
                float lResolution = ASensor_getResolution(mSensor);
                int32_t lMinDelay = ASensor_getMinDelay(mSensor);
                if (ASensorEventQueue_setEventRate(mEventLoop
                    .mSensorEventQueue, mSensor, lMinDelay) < 0) {
                    goto ERROR;
                }

                packt::Log::info("Activating sensor:");
                packt::Log::info("Name       : %s", lName);
                packt::Log::info("Vendor     : %s", lVendor);
                packt::Log::info("Resolution : %f", lResolution);
                packt::Log::info("Min Delay  : %d", lMinDelay);
            } else {
                packt::Log::error("No sensor type %d", mSensorType);
            }
        }
        return STATUS_OK;

    ERROR:
        Log::error("Error while activating sensor.");
        disable();
        return STATUS_KO;
    }

    status Sensor::disable() {
        if ((mEventLoop.mEnabled) && (mSensor != NULL)) {
            if (ASensorEventQueue_disableSensor(
                mEventLoop.mSensorEventQueue, mSensor) < 0) {
                goto ERROR;
            }
            mSensor = NULL;
        }
        return STATUS_OK;

    ERROR:
        Log::error("Error while deactivating sensor.");
        return STATUS_KO;
    }
}
