/*
 * Resource.cpp
 *
 *  Created on: 2012-7-13
 *      Author: mcxiaoke
 */

#include "Resource.hpp"
#include "Log.hpp"

namespace demo {

Resource::Resource(android_app* app, const char* path) :
        mPath(path), mAssetManager(app->activity->assetManager), mAsset(NULL) {
}

const char* Resource::getPath() {
    return mPath;
}

status Resource::open() {
    mAsset = AAssetManager_open(mAssetManager, mPath, AASSET_MODE_UNKNOWN);
    return (mAsset != NULL) ? STATUS_OK : STATUS_KO;
}

void Resource::close() {
    if (mAsset != NULL) {
        AAsset_close(mAsset);
        mAsset = NULL;
    }
}

status Resource::read(void* buffer, size_t size) {
    int32_t read = AAsset_read(mAsset, buffer, size);
    return (read == size) ? STATUS_OK : STATUS_KO;
}

off_t Resource::getLength(){
    return AAsset_getLength(mAsset);
}

const void* Resource::bufferize(){
    return AAsset_getBuffer(mAsset);
}

}

