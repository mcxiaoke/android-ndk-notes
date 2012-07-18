#ifndef _PACKT_INPUTSERVICE_HPP_
#define _PACKT_INPUTSERVICE_HPP_

#include "Context.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace demo {
    class InputService : public InputHandler {
    public:
        InputService(android_app* pApplication,
          const int32_t& pWidth, const int32_t& pHeight);

        float getHorizontal();
        float getVertical();
        void setRefPoint(Location* pTouchReference);

        status start();

    public:
        bool onTouchEvent(AInputEvent* pEvent);

    private:
        android_app* mApplication;

        // Input values.
        float mHorizontal, mVertical;

        // Reference point to evaluate touch distance.
        Location* mRefPoint;
        const int32_t& mWidth, &mHeight;
    };
}
#endif
