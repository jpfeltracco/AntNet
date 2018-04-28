//
// Created by jpfeltracco on 4/27/18.
//

#include <variant>

#include "AntNet/ObjectFactory.h"

#include "AntNet/System.h"

void System::add_component_handle(int handle) {
    components.push_back(handle);
}

std::vector<int>& System::get_component_handles() {
    return components;
}

RenderingSystem::RenderingSystem() {
//    auto& objs = ObjectFactory::get().get_gameobjects_list();
//
//    for (auto& game_object : objs) {
//        auto go_ptr = std::get_if<Tile>(&game_object);
//        if (go_ptr != nullptr)
//    }
}
