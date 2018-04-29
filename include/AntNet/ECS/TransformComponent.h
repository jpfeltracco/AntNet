//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_POSITIONCOMPONENT_H
#define ANTNET_POSITIONCOMPONENT_H

#include <AntNet/ECS/Components.h>
#include <AntNet/Vector2D.h>

class TransformComponent : public Component {
public:
    Vector2D position;
    Vector2D velocity;

    int height = 32;
    int width = 32;
    int scale = 1;

    int speed = 3;

    TransformComponent() { }

    TransformComponent(int sc) : scale(sc) { }

    TransformComponent(float x, float y) : position(x, y) { }

    TransformComponent(float x, float y, int h, int w, int scale)
        : position(x, y), height(h), width(w), scale(scale) { }

    void init() override {
        velocity.x = 0;
        velocity.y = 0;
    }

    void update() override {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};

#endif //ANTNET_POSITIONCOMPONENT_H
