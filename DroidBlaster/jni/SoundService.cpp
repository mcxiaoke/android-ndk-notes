#include "SoundService.hpp"
#include "Resource.hpp"
#include "Log.hpp"

namespace demo {
    SoundService::SoundService(android_app* pApplication) :
        mApplication(pApplication),
        mEngineObj(NULL), mEngine(NULL),
        mOutputMixObj(NULL),
        mBGMPlayerObj(NULL), mBGMPlayer(NULL), mBGMPlayerSeek(NULL) {
        Log::info("Creating SoundService.");
    }

    status SoundService::start() {
        Log::info("Starting SoundService.");
        SLresult lRes;
        const SLuint32      lEngineMixIIDCount = 1;
        const SLInterfaceID lEngineMixIIDs[]   = {SL_IID_ENGINE};
        const SLboolean     lEngineMixReqs[]   = {SL_BOOLEAN_TRUE};
        const SLuint32      lOutputMixIIDCount = 0;
        const SLInterfaceID lOutputMixIIDs[]   = {};
        const SLboolean     lOutputMixReqs[]   = {};

        // Creates OpenSL ES engine and dumps its capabilities.
        lRes = slCreateEngine(&mEngineObj, 0, NULL,
            lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mEngineObj)->Realize(mEngineObj,SL_BOOLEAN_FALSE);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE,
            &mEngine);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        // Creates audio output.
        lRes = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj,
            lOutputMixIIDCount, lOutputMixIIDs, lOutputMixReqs);
        lRes = (*mOutputMixObj)->Realize(mOutputMixObj,
            SL_BOOLEAN_FALSE);

        return STATUS_OK;

    ERROR:
        Log::error("Error while starting SoundService");
        stop();
        return STATUS_KO;
    }

    void SoundService::stop() {
        Log::info("Stopping SoundService.");

        // Stops and destroys BGM player.
        stopBGM();

        // Destroys audio output and engine.
        if (mOutputMixObj != NULL) {
            (*mOutputMixObj)->Destroy(mOutputMixObj);
            mOutputMixObj = NULL;
        }
        if (mEngineObj != NULL) {
            (*mEngineObj)->Destroy(mEngineObj);
            mEngineObj = NULL; mEngine = NULL;
        }
    }

    status SoundService::playBGM(const char* pPath) {
        SLresult lRes;
        Log::info("Opening BGM %s", pPath);

        // Set-up BGM audio source.
        Resource lResource(mApplication, pPath);
        ResourceDescriptor lDescriptor = lResource.descript();
        if (lDescriptor.mDescriptor < 0) {
            Log::info("Could not open BGM file");
            return STATUS_KO;
        }

        SLDataLocator_AndroidFD lDataLocatorIn;
        lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
        lDataLocatorIn.fd          = lDescriptor.mDescriptor;
        lDataLocatorIn.offset      = lDescriptor.mStart;
        lDataLocatorIn.length      = lDescriptor.mLength;

        SLDataFormat_MIME lDataFormat;
        lDataFormat.formatType    = SL_DATAFORMAT_MIME;
        lDataFormat.mimeType      = NULL;
        lDataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

        SLDataSource lDataSource;
        lDataSource.pLocator = &lDataLocatorIn;
        lDataSource.pFormat  = &lDataFormat;

        SLDataLocator_OutputMix lDataLocatorOut;
        lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        lDataLocatorOut.outputMix   = mOutputMixObj;

        SLDataSink lDataSink;
        lDataSink.pLocator = &lDataLocatorOut;
        lDataSink.pFormat  = NULL;

        // Creates BGM player and retrieves its interfaces.
        const SLuint32 lBGMPlayerIIDCount = 2;
        const SLInterfaceID lBGMPlayerIIDs[] =
            { SL_IID_PLAY, SL_IID_SEEK };
        const SLboolean lBGMPlayerReqs[] =
            { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

        lRes = (*mEngine)->CreateAudioPlayer(mEngine,
            &mBGMPlayerObj, &lDataSource, &lDataSink,
            lBGMPlayerIIDCount, lBGMPlayerIIDs, lBGMPlayerReqs);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayerObj)->Realize(mBGMPlayerObj,
            SL_BOOLEAN_FALSE);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,
            SL_IID_PLAY, &mBGMPlayer);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,
            SL_IID_SEEK, &mBGMPlayerSeek);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        // Enables looping and starts playing.
        lRes = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek,
            SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayer)->SetPlayState(mBGMPlayer,
            SL_PLAYSTATE_PLAYING);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        return STATUS_OK;

    ERROR:
        return STATUS_KO;
    }

    void SoundService::stopBGM() {
        if (mBGMPlayer != NULL) {
            SLuint32 lBGMPlayerState;
            (*mBGMPlayerObj)->GetState(mBGMPlayerObj,
                &lBGMPlayerState);
            if (lBGMPlayerState == SL_OBJECT_STATE_REALIZED) {
                (*mBGMPlayer)->SetPlayState(mBGMPlayer,
                    SL_PLAYSTATE_PAUSED);

                (*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
                mBGMPlayerObj = NULL;
                mBGMPlayer = NULL; mBGMPlayerSeek = NULL;
            }
        }
    }
}
