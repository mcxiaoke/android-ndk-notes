/*
 * DroidBlaster.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "DroidBlaster.hpp"
#include "Log.hpp"

#include <unistd.h>

namespace db {

DroidBlaster::DroidBlaster() {
	demo::Log::info("Creating DroidBlaster.");
}

DroidBlaster::~DroidBlaster() {
	demo::Log::info("Destructing DroidBlaster.");
}

demo::status DroidBlaster::onActivate() {
	demo::Log::info("Activating DroidBlaster.");
	return demo::STATUS_OK;
}

void DroidBlaster::onDeactivate() {
	demo::Log::info("Deactivating DroidBlaster.");
}

demo::status DroidBlaster::onStep() {
	demo::Log::info("Starting step.");
	usleep(300000);
	demo::Log::info("Steping done..");
	return demo::STATUS_OK;
}

void DroidBlaster::onStart() {
	demo::Log::info("onStart.");
}

void DroidBlaster::onResume() {
	demo::Log::info("onResume.");
}
void DroidBlaster::onPause() {
	demo::Log::info("onPause.");
}
void DroidBlaster::onStop() {
	demo::Log::info("onStop.");
}
void DroidBlaster::onDestroy() {
	demo::Log::info("onDestroy.");
}

void DroidBlaster::onSaveState(void** data, size_t* size) {
	demo::Log::info("onSaveState.");
}
void DroidBlaster::onConfigurationChanged() {
	demo::Log::info("onConfigurationChanged.");
}
void DroidBlaster::onLowMemory() {
	demo::Log::info("onLowMemory.");
}

void DroidBlaster::onCreateWindow() {
	demo::Log::info("onCreateWindow.");
}
void DroidBlaster::onDestroyWindow() {
	demo::Log::info("onDestroyWindow.");
}
void DroidBlaster::onGainFocus() {
	demo::Log::info("onGainFocus.");
}
void DroidBlaster::onLostFocus() {
	demo::Log::info("onLostFocus.");
}

}

