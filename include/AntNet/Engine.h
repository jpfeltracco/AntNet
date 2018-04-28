//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_ENGINE_H
#define ANTNET_ENGINE_H

#include <memory>
#include <vector>

#include "System.h"

class Engine
{
public:
    void update(float dt);
    void add(std::unique_ptr<System> sys_ptr);
private:
    std::vector<std::unique_ptr<System>> systems;
};

#endif //ANTNET_ENGINE_H
