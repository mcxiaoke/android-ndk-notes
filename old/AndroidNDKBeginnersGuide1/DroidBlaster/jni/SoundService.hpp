#ifndef _PACKT_SOUNDSERVICE_HPP_
#define _PACKT_SOUNDSERVICE_HPP_

#include "Sound.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

namespace packt {
    class SoundService {
    public:
        SoundService(android_app* pApplication);
        ~SoundService();

        status start();
        void stop();

        status playBGM(const char* pPath);
        void stopBGM();

        Sound* registerSound(const char* pPath);
        void playSound(Sound* pSound);

    private:
        status startSoundPlayer();

    private:
        android_app* mApplication;

        // OpenSL ES engine.
        SLObjectItf mEngineObj; SLEngineItf mEngine;
        // Audio output.
        SLObjectItf mOutputMixObj;

        // Background music player.
        SLObjectItf mBGMPlayerObj; SLPlayItf mBGMPlayer;
        SLSeekItf mBGMPlayerSeek;

        // Sound player.
        SLObjectItf mPlayerObj; SLPlayItf mPlayer;
        SLBufferQueueItf mPlayerQueue;
        // Sounds.
        Sound* mSounds[32]; int32_t mSoundCount;
    };
}
#endif
