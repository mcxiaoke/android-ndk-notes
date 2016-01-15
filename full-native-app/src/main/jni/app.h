#ifndef __APP_H__
#define __APP_H__

#include "activity_handler.h"
#include "event_loop.h"
#include "graphics_manager.h"
#include "ship.h"
#include "types.h"


class MainApp: public ActivityHandler {
public:
    MainApp(android_app* app);
    void run();
protected:
    status onActivate();
    void onDeactivate();

    status onStep();

    void onStart();
    void onResume();
    void onPause();
    void onStop();
    void onDestroy();

    void onSaveInstanceState(void** pDate, size_t* pSize);
    void onConfigurationChanged();
    void onLowMemory();

    void onCreateWindow();
    void onDestroyWindow();
    void onGainFocus();
    void onLostFocus();
private:
    GraphicsManager mGraphicsManager;
    EventLoop mEventLoop;
    Ship mShip;
};

#endif
