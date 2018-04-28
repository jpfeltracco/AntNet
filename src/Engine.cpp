//
// Created by jpfeltracco on 4/27/18.
//

#include <AntNet/Engine.h>
#include <AntNet/System.h>

void Engine::update(float dt) {

}

void Engine::add(std::unique_ptr<System> sys_ptr) {
    systems.push_back(std::move(sys_ptr));
}
