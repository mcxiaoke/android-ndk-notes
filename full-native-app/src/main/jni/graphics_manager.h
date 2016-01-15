#ifndef _GRAPHICS_MANAGER_H_
#define _GRAPHICS_MANAGER_H_

#include "types.h"
#include <android_native_app_glue.h>

struct GraphicsElement {
    GraphicsElement(int32_t w, int32_t h):
        location(), width(w), height(h) {

    }
    Location location;
    int32_t width;
    int32_t height;
};

class GraphicsManager {
public:
    GraphicsManager(android_app* app);
    ~GraphicsManager();

    int32_t getRenderWidth() {return mRenderWidth;}
    int32_t getRenderHeight() {return mRenderHeight;}
    GraphicsElement* registerElement(int32_t w, int32_t h);

    status start();
    status update();
private:
    android_app* mApp;
    int32_t mRenderWidth;
    int32_t mRenderHeight;
    GraphicsElement* mElements[1024];
    int32_t mElementCount;
};

#endif
