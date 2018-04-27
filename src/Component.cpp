//
// Created by jpfeltracco on 4/27/18.
//

#include <AntNet/Component.h>

Pose::Pose()
        : x(0), y(0), d(Direction::North) { }

Pose::Pose(int x, int y, Direction d)
        : x(x), y(y), d(d) { }

Sprite::Sprite() : texture_handle(-1) {}

Sprite::Sprite(int texture_handle) : texture_handle(texture_handle) {}
