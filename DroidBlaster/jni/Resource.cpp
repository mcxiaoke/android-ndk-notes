#include "Resource.hpp"
#include "Log.hpp"

namespace packt {
    Resource::Resource(android_app* pApplication, const char* pPath):
        mPath(pPath),
        mAssetManager(pApplication->activity->assetManager),
        mAsset(NULL) {
    }

    status Resource::open() {
        mAsset = AAssetManager_open(mAssetManager, mPath,
                                    AASSET_MODE_UNKNOWN);
        return (mAsset != NULL) ? STATUS_OK : STATUS_KO;
    }

    void Resource::close() {
        if (mAsset != NULL) {
            AAsset_close(mAsset);
            mAsset = NULL;
        }
    }

    status Resource::read(void* pBuffer, size_t pCount) {
        int32_t lReadCount = AAsset_read(mAsset, pBuffer, pCount);
        return (lReadCount == pCount) ? STATUS_OK : STATUS_KO;
    }

    const char* Resource::getPath() {
        return mPath;
    }

    off_t Resource::getLength() {
        return AAsset_getLength(mAsset);
    }

    const void* Resource::bufferize() {
        return AAsset_getBuffer(mAsset);
    }

    ResourceDescriptor Resource::descript() {
        ResourceDescriptor lDescriptor = { -1, 0, 0 };
        AAsset* lAsset = AAssetManager_open(mAssetManager, mPath,
                                            AASSET_MODE_UNKNOWN);
        if (lAsset != NULL) {
            lDescriptor.mDescriptor = AAsset_openFileDescriptor(
                lAsset, &lDescriptor.mStart, &lDescriptor.mLength);
            AAsset_close(lAsset);
        }
        return lDescriptor;
    }
}
