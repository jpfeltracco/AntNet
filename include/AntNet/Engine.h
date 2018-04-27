//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_ENGINE_H
#define ANTNET_ENGINE_H

#include <vector>

#include "System.h"

class Engine
{
public:
    void update(float dt);
    void loop();
    void add(System* sys);
private:
    std::vector<System> systems;
};

#endif //ANTNET_ENGINE_H
