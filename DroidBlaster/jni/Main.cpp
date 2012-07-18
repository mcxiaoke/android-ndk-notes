/*
 * Main.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "Context.hpp"
#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "InputService.hpp"
#include "SoundService.hpp"
#include "TimeService.hpp"

void android_main(android_app* pApplication) {
    // Creates services.
    demo::TimeService lTimeService;
    demo::GraphicsService lGraphicsService(pApplication,
        &lTimeService);
    demo::InputService lInputService(pApplication,
        lGraphicsService.getWidth(), lGraphicsService.getHeight());
    demo::SoundService lSoundService(pApplication);

    // Fills the context.
    demo::Context lContext = { &lGraphicsService, &lInputService,
        &lSoundService, &lTimeService };

    // Starts the game loop.
    demo::EventLoop lEventLoop(pApplication);
    db::DroidBlaster lDroidBlaster(&lContext);
    lEventLoop.run(&lDroidBlaster, &lInputService);
}

