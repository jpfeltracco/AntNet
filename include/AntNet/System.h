//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_SYSTEM_H
#define ANTNET_SYSTEM_H

#include <variant>
#include <vector>

#include "Component.h"

class System {
public:
    void add_component_handle(int handle);
    std::vector<int>& get_component_handles();
private:
    std::vector<int> components;
};

class RenderingSystem : public System {
public:
    RenderingSystem();
};

#endif //ANTNET_SYSTEM_H
