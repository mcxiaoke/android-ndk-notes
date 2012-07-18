#include "InputService.hpp"
#include "Log.hpp"

#include <android_native_app_glue.h>
#include <cmath>

namespace demo {
    InputService::InputService(android_app* pApplication,
        const int32_t& pWidth, const int32_t& pHeight) :
        mApplication(pApplication),
        mHorizontal(0.0f), mVertical(0.0f),
        mRefPoint(NULL), mWidth(pWidth), mHeight(pHeight)
    {}

    float InputService::getHorizontal() {
        return mHorizontal;
    }

    float InputService::getVertical() {
        return mVertical;
    }

    void InputService::setRefPoint(Location* pTouchReference) {
        mRefPoint = pTouchReference;
    }

    status InputService::start() {
        Log::info("Starting InputService.");

        mHorizontal = 0.0f, mVertical = 0.0f;
        if ((mWidth == 0) || (mHeight == 0)) {
            return STATUS_KO;
        }
        return STATUS_OK;
    }

    bool InputService::onTouchEvent(AInputEvent* pEvent) {
#ifdef INPUTSERVICE_LOG_EVENTS
         Log::debug("AMotionEvent_getAction=%d", AMotionEvent_getAction(pEvent));
         Log::debug("AMotionEvent_getFlags=%d", AMotionEvent_getFlags(pEvent));
         Log::debug("AMotionEvent_getMetaState=%d", AMotionEvent_getMetaState(pEvent));
         Log::debug("AMotionEvent_getEdgeFlags=%d", AMotionEvent_getEdgeFlags(pEvent));
         Log::debug("AMotionEvent_getDownTime=%lld", AMotionEvent_getDownTime(pEvent));
         Log::debug("AMotionEvent_getEventTime=%lld", AMotionEvent_getEventTime(pEvent));
         Log::debug("AMotionEvent_getXOffset=%f", AMotionEvent_getXOffset(pEvent));
         Log::debug("AMotionEvent_getYOffset=%f", AMotionEvent_getYOffset(pEvent));
         Log::debug("AMotionEvent_getXPrecision=%f", AMotionEvent_getXPrecision(pEvent));
         Log::debug("AMotionEvent_getYPrecision=%f", AMotionEvent_getYPrecision(pEvent));
         Log::debug("AMotionEvent_getPointerCount=%d", AMotionEvent_getPointerCount(pEvent));
         Log::debug("AMotionEvent_getRawX=%f", AMotionEvent_getRawX(pEvent, 0));
         Log::debug("AMotionEvent_getRawY=%f", AMotionEvent_getRawY(pEvent, 0));
         Log::debug("AMotionEvent_getX=%f", AMotionEvent_getX(pEvent, 0));
         Log::debug("AMotionEvent_getY=%f", AMotionEvent_getY(pEvent, 0));
         Log::debug("AMotionEvent_getPressure=%f", AMotionEvent_getPressure(pEvent, 0));
         Log::debug("AMotionEvent_getSize=%f", AMotionEvent_getSize(pEvent, 0));
         Log::debug("AMotionEvent_getOrientation=%f", AMotionEvent_getOrientation(pEvent, 0));
         Log::debug("AMotionEvent_getTouchMajor=%f", AMotionEvent_getTouchMajor(pEvent, 0));
         Log::debug("AMotionEvent_getTouchMinor=%f", AMotionEvent_getTouchMinor(pEvent, 0));
#endif
        const float TOUCH_MAX_RANGE = 65.0f; // In pixels.

        if (mRefPoint != NULL) {
            if (AMotionEvent_getAction(pEvent)
                            == AMOTION_EVENT_ACTION_MOVE) {
                // Needs a conversion to proper coordinates
                // (origin at bottom/left). Only lMoveY needs it.
                float lMoveX = AMotionEvent_getX(pEvent, 0)
                             - mRefPoint->mPosX;
                float lMoveY = mHeight - AMotionEvent_getY(pEvent, 0)
                             - mRefPoint->mPosY;
                float lMoveRange = sqrt((lMoveX * lMoveX)
                                      + (lMoveY * lMoveY));

                if (lMoveRange > TOUCH_MAX_RANGE) {
                    float lCropFactor = TOUCH_MAX_RANGE / lMoveRange;
                    lMoveX *= lCropFactor; lMoveY *= lCropFactor;
                }

                mHorizontal = lMoveX / TOUCH_MAX_RANGE;
                mVertical   = lMoveY / TOUCH_MAX_RANGE;
            } else {
                mHorizontal = 0.0f; mVertical = 0.0f;
            }
        }
        return true;
    }
}
