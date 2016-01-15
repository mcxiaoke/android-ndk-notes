/*
* @Author: mcxiaoke
* @Date:   2016-01-15 22:14:37
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 23:21:06
*/

#include "app.h"
#include "log.h"
#include <unistd.h>

static const int32_t SHIP_SIZE = 64;

MainApp::MainApp(android_app* app):
    mGraphicsManager(app),
    mEventLoop(app, *this),
    mShip(app, mGraphicsManager) {
    Log::info("Creating MainApp");
    GraphicsElement* shipGraphics = mGraphicsManager.registerElement(SHIP_SIZE, SHIP_SIZE);
    mShip.registerShip(shipGraphics);
}

void MainApp::run() {
    mEventLoop.run();
}

status MainApp::onActivate() {
    Log::info("Activating MainApp");
    if (mGraphicsManager.start() != STATUS_OK) {
        return STATUS_KO;
    }
    mShip.initialize();
    return STATUS_OK;
}

void MainApp::onDeactivate() {
    Log::info("Deactivating MainApp");
}

status MainApp::onStep() {
    // Log::info("Starting step");
    // usleep(300000);
    // Log::info("Stepping done");
    // return STATUS_OK;
    return mGraphicsManager.update();
}

void MainApp::onStart() {
    Log::info("onStart");
}

void MainApp::onResume() {
    Log::info("onResume");
}

void MainApp::onPause() {
    Log::info("onPause");
}

void MainApp::onStop() {
    Log::info("onStop");
}

void MainApp::onDestroy() {
    Log::info("onDestroy");
}

void MainApp::onSaveInstanceState(void** pDate, size_t* pSize) {
    Log::info("onSaveInstanceState");
}

void MainApp::onConfigurationChanged() {
    Log::info("onConfigurationChanged");
}

void MainApp::onLowMemory() {
    Log::info("onLowMemory");
}

void MainApp::onCreateWindow() {
    Log::info("onCreateWindow");
}

void MainApp::onDestroyWindow() {
    Log::info("onDestroyWindow");
}

void MainApp::onGainFocus() {
    Log::info("onGainFocus");
}

void MainApp::onLostFocus() {
    Log::info("onLostFocus");
}

