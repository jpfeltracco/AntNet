//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_SYSTEM_H
#define ANTNET_SYSTEM_H

#include <variant>
#include <vector>

#include "Component.h"

class System {
private:
    std::vector<std::variant<Pose, Sprite>> components;
};

class RenderingSystem : System {

};

#endif //ANTNET_SYSTEM_H
