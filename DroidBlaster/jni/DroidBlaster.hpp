/*
 * DroidBlaster.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef DROIDBLASTER_HPP_
#define DROIDBLASTER_HPP_

#include "EventHandler.hpp"
#include "Background.hpp"
#include "Context.hpp"
#include "GraphicsService.hpp"
#include "InputService.hpp"
#include "Sound.hpp"
#include "SoundService.hpp"
#include "Ship.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace db {
class DroidBlaster: public demo::EventHandler {
public:
    DroidBlaster(demo::Context *context);
    ~DroidBlaster();

protected:
    demo::status onActivate();
    void onDeactivate();
    demo::status onStep();

    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();

    void onSaveState(void** data, size_t* size);
    void onConfigurationChanged();
    void onLowMemory();

    void onCreateWindow();
    void onDestroyWindow();
    void onGainFocus();
    void onLostFocus();
//
//private:
//    void clear();
//    void drawCursor(int size, int px, int py);

private:
//    android_app *mApp;
//    ANativeWindow_Buffer mWindowBuffer;
    demo::InputService *mInputService;
    demo::GraphicsService *mGraphicsService;
    demo::SoundService *mSoundService;
    demo::TimeService *mTimeService;
    Background mBackground;
    Ship mShip;
    demo::Sound *mStartSound;

//    bool mInitialized;

//    float mPosX;
//    float mPosY;
//    const int32_t mSize;
//    const float mSpeed;

};

}

#endif /* DROIDBLASTER_HPP_ */
