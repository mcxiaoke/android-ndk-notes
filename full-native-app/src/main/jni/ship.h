#ifndef _SHIP_H_
#define _SHIP_H_
#pragma once

#include "graphics_manager.h"

class Ship {
public:
    Ship(android_app* app, GraphicsManager& gm);
    void registerShip(GraphicsElement* graphics);
    void initialize();
private:
    GraphicsManager& mGraphicsManager;
    GraphicsElement* mGraphics;
};


#endif
