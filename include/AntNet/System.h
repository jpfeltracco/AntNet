//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_SYSTEM_H
#define ANTNET_SYSTEM_H

#include <variant>
#include <vector>

#include <AntNet/Component.h>
#include <AntNet/Handle.h>

class System : HandleChangeReceiver {
public:
    void add_component_handle(Handle handle);
    void remove_component_handle(Handle handle);
    std::vector<Handle>& get_component_handles();

    void notify(Handle h, bool removed) override {};
private:
    std::vector<Handle> components;
};

class RenderingSystem : public System {
public:
    RenderingSystem();

    void notify(Handle h, bool removed) override;
private:
    std::vector<Handle> rendering_gameobjects;
};

#endif //ANTNET_SYSTEM_H
