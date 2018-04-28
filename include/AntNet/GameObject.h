//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_GAMEOBJECT_H
#define ANTNET_GAMEOBJECT_H

#include <variant>
#include <vector>

#include <AntNet/Component.h>
#include <AntNet/Handle.h>

class GameObject {
public:
    GameObject();
    void update();
    void add_component(Handle comp_handle);
private:
    std::vector<Handle> components;
};

class Tile : GameObject {
public:
    Tile();
    Tile(int x, int y, int texture_handle);
};

using gameobject_t = std::variant<std::monostate, Tile>;

#endif //ANTNET_GAMEOBJECT_H
