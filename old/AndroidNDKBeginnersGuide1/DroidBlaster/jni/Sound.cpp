#include "Sound.hpp"
#include "Log.hpp"

#include <png.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

namespace packt {
    Sound::Sound(android_app* pApplication, const char* pPath) :
        mResource(pApplication, pPath),
        mBuffer(NULL), mLength(0) {
    }

    const char* Sound::getPath() {
        return mResource.getPath();
    }

    status Sound::load() {
        Log::info("Loading sound %s", mResource.getPath());
        status lRes;

        // Opens sound file.
        if (mResource.open() != STATUS_OK) {
            return STATUS_KO;
        }

        // Reads sound file.
        mLength = mResource.getLength();
        mBuffer = new uint8_t[mLength];
        lRes = mResource.read(mBuffer, mLength);
        mResource.close();

        if (lRes != STATUS_OK) {
            Log::error("Error while reading PCM sound.");
            return STATUS_KO;
        } else {
            return STATUS_OK;
        }
    }

    status Sound::unload() {
        delete[] mBuffer;
        mBuffer = NULL; mLength = 0;

        return STATUS_OK;
    }
}
