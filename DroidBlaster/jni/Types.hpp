/*
 * Types.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <stdint.h>

// hack from https://groups.google.com/forum/?fromgroups#!topic/android-ndk/Inip9SFiRZs
// and here http://www.opengl.org/registry/ABI/#5
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

namespace demo {
typedef int32_t status;

const status STATUS_OK = 0;
const status STATUS_KO = -1;
const status STATUS_EXIT = -2;

struct Location {
    Location() :
            mPosX(0), mPosY(0) {
    }
    ;
    void setPosition(float px, float py) {
        mPosX = px;
        mPosY = py;
    }

    void translate(float ix, float iy) {
        mPosX += ix;
        mPosY += iy;
    }

    float mPosX;
    float mPosY;
};

}

#endif /* TYPES_HPP_ */
