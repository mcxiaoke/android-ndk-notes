/*
 * GraphicsService.cpp
 *
 *  Created on: 2012-7-12
 *      Author: mcxiaoke
 */

#include "GraphicsService.hpp"
#include "Log.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <math.h>

namespace demo {
GraphicsService::GraphicsService(android_app *app, TimeService *ts) :
        mApp(app), mTimeService(ts), mWidth(0), mHeight(0), mDisplay(
                EGL_NO_DISPLAY ), mSurface(EGL_NO_SURFACE ), mContext(
                EGL_NO_CONTEXT ), mTextures(), mTextureCount(0) {
    Log::debug("Creating GraphicsService.");
}

GraphicsService::~GraphicsService() {
    for (int32_t i = 0; i < mTextureCount; ++i) {
        delete mTextures[i];
        mTextures[i] = NULL;
    }
    mTextureCount = 0;
}

const int32_t& GraphicsService::getHeight() {
    return mHeight;
}

const int32_t& GraphicsService::getWidth() {
    return mWidth;
}

status GraphicsService::start() {
    Log::debug("GraphicsService::start()");
    EGLint format;
    EGLint numConfigs;
    EGLint error;
    EGLConfig eglConfig;
    const EGLint attrs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
    Log::debug("GraphicsService::start() Connecting to the display.");
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY );
    if (mDisplay == EGL_NO_DISPLAY ) {
        Log::error("GraphicsService::start() eglGetDisplay failed.");
        goto ERROR;
    }
    if (!eglInitialize(mDisplay, NULL, NULL)) {
        Log::error("GraphicsService::start() eglInitialize failed.");
        goto ERROR;
    }
    if (!eglChooseConfig(mDisplay, attrs, &eglConfig, 1, &numConfigs)
            || (numConfigs <= 0)) {
        Log::error("GraphicsService::start() eglChooseConfig failed.");
        goto ERROR;
    }
    if (!eglGetConfigAttrib(mDisplay, eglConfig, EGL_NATIVE_VISUAL_ID,
            &format)) {
        Log::error("GraphicsService::start() eglGetConfigAttrib failed.");
        goto ERROR;
    }
    ANativeWindow_setBuffersGeometry(mApp->window, 0, 0, format);

    mSurface = eglCreateWindowSurface(mDisplay, eglConfig, mApp->window, NULL);
    if (mSurface == EGL_NO_SURFACE ) {
        Log::error("GraphicsService::start() eglCreateWindowSurface failed.");
        goto ERROR;
    }
    mContext = eglCreateContext(mDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
    if (mContext == EGL_NO_CONTEXT ) {
        Log::error("GraphicsService::start() eglCreateContext failed.");
        goto ERROR;
    }

    Log::debug("GraphicsService::start() Activating the display.");
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
            || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
            || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
            || (mWidth <= 0) || (mHeight <= 0)) {
        goto ERROR;
        Log::error("GraphicsService::start() eglMakeCurrent failed.");
    }

    glViewport(0, 0, mWidth, mHeight);
    Log::debug("GraphicsService::start() glViewport status ok.");

    // Displays information about OpenGL.
    Log::debug("Version  : %s", glGetString(GL_VERSION));
    Log::debug("Vendor   : %s", glGetString(GL_VENDOR));
    Log::debug("Renderer : %s", glGetString(GL_RENDERER));
    Log::debug("Viewport : %d x %d", mWidth, mHeight);

    if (loadResources() != STATUS_OK) {
        goto ERROR;
    }

    return STATUS_OK;

    ERROR: Log::error("Error while starting GraphicsService.");
    stop();
    return STATUS_KO;

}

void GraphicsService::stop() {
    Log::debug("GraphicsService::stop().");
    unloadResources();
    if (mDisplay != EGL_NO_DISPLAY ) {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                EGL_NO_CONTEXT );
        if (mContext != NULL) {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }
        if (mSurface != EGL_NO_SURFACE ) {
            eglDestroySurface(mDisplay, mSurface);
            mSurface = EGL_NO_SURFACE;
        }
        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

status GraphicsService::update() {
    float timeStep = mTimeService->elapsed();
    static float color = 0.0f;
    color += timeStep * 0.2f;
    if (color > 1.0f) {
        color -= 1.0f;
    }
    Log::info("GraphicsService::update Color: %f", color);
    glClearColor(color, color, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
        Log::error("Error %d swapping buffers.", eglGetError());
        return STATUS_KO;
    }
    return STATUS_OK;
}

status GraphicsService::loadResources() {
    for (int32_t i = 0; i < mTextureCount; ++i) {
        if (mTextures[i]->load() != STATUS_OK) {
            return STATUS_KO;
        }
    }
    return STATUS_OK;
}

status GraphicsService::unloadResources() {
    for (int32_t i = 0; i < mTextureCount; ++i) {
        mTextures[i]->unload();
    }
    return STATUS_OK;
}

GraphicsTexture* GraphicsService::registerTexture(const char* path) {
    for (int32_t i = 0; i < mTextureCount; ++i) {
        if (strcmp(path, mTextures[i]->getPath()) != 0) {
            return mTextures[i];
        }
    }

    GraphicsTexture* texture = new GraphicsTexture(mApp, path);
    mTextures[mTextureCount++] = texture;
    return texture;

}

}

