//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_COMPONENT_H
#define ANTNET_COMPONENT_H

#include <variant>

class Component {
public:
    enum class Type { Pose = 0, Sprite, Count };
};

class Pose : Component {
public:
    enum class Direction { North, South, East, West };
public:
    Pose();
    Pose(int x, int y, Direction d);
    int x, y;
    Direction d;
};

class Sprite : Component {
public:
    Sprite();
    explicit Sprite(int texture_handle);
    int texture_handle;
};

using component_t = std::variant<Pose, Sprite>;

#endif //ANTNET_COMPONENT_H
