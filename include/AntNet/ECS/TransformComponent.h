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

    int speed = 3;

    TransformComponent() : position(0.0f, 0.0f) { }
    TransformComponent(float x, float y) : position(x, y) { }

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
