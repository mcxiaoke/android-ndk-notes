#ifndef PACKT_GRAPHICSOBJECT_HPP
#define PACKT_GRAPHICSOBJECT_HPP

#include "Types.hpp"

#include <boost/shared_ptr.hpp>
#include <irrlicht.h>
#include <vector>

namespace packt {
    class GraphicsObject {
    public:
        typedef boost::shared_ptr<GraphicsObject> ptr;
        typedef std::vector<ptr> vec;
        typedef vec::iterator vec_it;

    public:
        GraphicsObject(const char* pTexture, const char* pMesh,
            Location* pLocation);

        void spin(float pX, float pY, float pZ);

        void initialize(irr::scene::ISceneManager* pSceneManager);
        void update();

    private:
        Location* mLocation;
        irr::scene::ISceneNode* mNode;
        irr::io::path mTexture; irr::io::path mMesh;
    };
}
#endif
