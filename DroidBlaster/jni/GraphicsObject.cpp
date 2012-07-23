#include "GraphicsObject.hpp"
#include "Log.hpp"

namespace packt {
    GraphicsObject::GraphicsObject(const char* pTexture,
      const char* pMesh, Location* pLocation) :
      mLocation(pLocation), mNode(NULL),
      mTexture(pTexture), mMesh(pMesh)
    {}

    void GraphicsObject::spin(float pX, float pY, float pZ) {
        mNode->removeAnimators();
        irr::scene::ISceneNodeAnimator* lAnimator =
            mNode->getSceneManager()->createRotationAnimator(
                irr::core::vector3df(pX, pY, pZ));
        mNode->addAnimator(lAnimator);
        lAnimator->drop();
    }

    void GraphicsObject::initialize(
        irr::scene::ISceneManager* pSceneManager) {
        irr::scene::IAnimatedMesh* lMesh =
            pSceneManager->getMesh(mMesh);
        irr::video::ITexture* lTexture = pSceneManager->
            getVideoDriver()->getTexture(mTexture);

        mNode = pSceneManager->addMeshSceneNode(lMesh);
        mNode->setMaterialTexture(0, lTexture);
        mNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    }

    void GraphicsObject::update() {
        mNode->setPosition(irr::core::vector3df(
            mLocation->mPosX, 0.0f, mLocation->mPosY));
    }
}
