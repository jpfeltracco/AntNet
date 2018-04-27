//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_GAMEOBJECT_H
#define ANTNET_GAMEOBJECT_H

#include <array>
#include <variant>

#include "Component.h"

class GameObject {
public:
    GameObject();
    void update();
    int get_component_handle(Component::Type type);
    void set_component_handle(Component::Type type, int handle);
private:
    std::array<int, static_cast<std::size_t>(Component::Type::Count)> components;
};

class Tile : GameObject {
public:
    Tile();
    Tile(int x, int y, Pose::Direction d, int texture_handle);
};

using gameobject_t = std::variant<Tile>;

#endif //ANTNET_GAMEOBJECT_H
