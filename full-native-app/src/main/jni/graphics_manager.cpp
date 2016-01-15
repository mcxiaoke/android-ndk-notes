/*
* @Author: mcxiaoke
* @Date:   2016-01-15 22:51:49
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 23:22:39
*/

#include "graphics_manager.h"
#include "log.h"

GraphicsManager::GraphicsManager(android_app* app):
    mApp(app),
    mRenderWidth(0), mRenderHeight(0),
    mElements(), mElementCount(0) {
    Log::info("Creating GraphicsManager.");
}

GraphicsManager::~GraphicsManager() {
    Log::info("Destroying GraphicsManager.");
    for (int32_t i = 0; i < mElementCount; ++i) {
        delete mElements[i];
    }
}

GraphicsElement* GraphicsManager::registerElement(int32_t w, int32_t h) {
    mElements[mElementCount] = new GraphicsElement(w, h);
    return mElements[mElementCount++];
}


status GraphicsManager::start() {
    Log::info("Starting GraphicsManager.");
    // forces 32bit format
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_setBuffersGeometry(mApp->window,
                                         0, 0, WINDOW_FORMAT_RGBX_8888) < 0) {
        Log::error("Error while setting buffer geometry.");
        return STATUS_KO;
    }
    // need to lock window buffer to get its properties
    if (ANativeWindow_lock(mApp->window, &windowBuffer, NULL) >= 0) {
        mRenderWidth = windowBuffer.width;
        mRenderHeight = windowBuffer.height;
        ANativeWindow_unlockAndPost(mApp->window);
    } else {
        Log::error("Error while locking window.");
        return STATUS_KO;
    }
    return STATUS_OK;
}


status GraphicsManager::update() {
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(mApp->window, &windowBuffer, NULL) < 0) {
        Log::error("Error while updating GraphicsManager.");
        return STATUS_KO;
    }
    // clear the window
    memset(windowBuffer.bits, 0,
           windowBuffer.stride * windowBuffer.height * sizeof(uint32_t*));

    // render graphics elements
    int32_t maxX = windowBuffer.width - 1;
    int32_t maxY = windowBuffer.height - 1;
    for (int32_t i = 0; i < mElementCount; ++i) {
        GraphicsElement* element = mElements[i];
        // compute coordinates
        int32_t lx = element->location.x - element->width / 2;
        int32_t rx = element->location.x + element->width / 2;
        int32_t ly = windowBuffer.height - element->location.y - element->height / 2;
        int32_t ry = windowBuffer.height - element->location.y + element->height / 2;
        // clip coordinates
        if (rx < 0 || lx > maxX
                || ry < 0 || ly > maxY) {
            continue;
        }
        if (lx < 0) {
            lx = 0;
        } else if (rx > maxX) {
            rx = maxX;
        }
        if (ly < 0) {
            ly = 0;
        } else if (ry > maxY) {
            ry = maxY;
        }
        // Draw a rectangle
        uint32_t* line = (uint32_t*)(windowBuffer.bits) + (windowBuffer.stride * ly);
        for (int iy = ly; iy <= ry; iy++) {
            for (int ix = lx; ix <= rx; ix++) {
                line[ix] = 0x000000ff;
            }
            line = line + windowBuffer.stride;
        }
        // finish drawing
        ANativeWindow_unlockAndPost(mApp->window);
        return STATUS_OK;
    }
}
