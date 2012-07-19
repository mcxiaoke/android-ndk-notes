#ifndef _PACKT_TYPES_HPP_
#define _PACKT_TYPES_HPP_

#include <stdint.h>
#include <cstdlib>

#ifndef __ANDROID__
#define __ANDROID__
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#define RAND(pMax) (float(pMax) * float(rand()) / float(RAND_MAX))

namespace packt {
    typedef int32_t status;

    const status STATUS_OK   = 0;
    const status STATUS_KO   = -1;
    const status STATUS_EXIT = -2;

    struct Location {
        Location(): mPosX(0.0f), mPosY(0.0f) {};

        void setPosition(float pPosX, float pPosY)
        { mPosX = pPosX; mPosY = pPosY; }

        void translate(float pAmountX, float pAmountY)
        { mPosX += pAmountX; mPosY += pAmountY; }

        float mPosX; float mPosY;
    };
}
#endif
