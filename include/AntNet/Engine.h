//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_ENGINE_H
#define ANTNET_ENGINE_H

#include <memory>
#include <vector>

#include <AntNet/Handle.h>
#include <AntNet/System.h>
#include <AntNet/ObjectFactory.h>

class Engine : public HandleChangeReceiver
{
public:
    static Engine& get() {
        static Engine instance;
        return instance;
    }

    Engine(const Engine& o) = delete;
    void operator=(const Engine& o) = delete;

    void update(float dt);
    void add(std::unique_ptr<System> sys_ptr);

    void notify(Handle h, bool removed);

private:
    Engine() = default;
    std::vector<std::unique_ptr<System>> systems;
};

#endif //ANTNET_ENGINE_H
