//
// Created by jpfeltracco on 4/27/18.
//

#include <variant>
#include <iostream>
#include <utility>

#include "AntNet/ObjectFactory.h"

#include "AntNet/System.h"

void System::add_component_handle(Handle handle) {
    components.push_back(handle);
}

void System::remove_component_handle(Handle handle) {
//    auto comp_iter = std::find(components.begin(), components.end(), handle);
//
//    if (comp_iter != components.end()) {
//        components.erase(comp_iter);
//    } else {
//        // not an error case
////        std::cout << "System::remove_component_handle: Tried to delete "
////                  << "handle not in components list." << std::endl;
//    }
}

std::vector<Handle>& System::get_component_handles() {
    return components;
}

RenderingSystem::RenderingSystem() {
}

void RenderingSystem::notify(Handle h, bool removed) {
    if (removed) {
        remove_component_handle(h);
        return;
    }

    auto obj_variant = factory::gameobject().get_object(h);
    auto obj_ptr = std::get_if<std::monostate>(&obj_variant);


//    if (obj_ptr == nullptr) {
//        std::visit([](Tile& arg) {
//            return arg.get_component_handles();
//        }, obj_variant);
//    }

//    std::visit([](auto&& arg) {
//        using T = std::decay_t<decltype(arg)>;
////        if constexpr(std::is_same_v<T, GameObject>)
//    });

//    if (obj_ptr != nullptr) {
//        auto& obj = *obj_ptr;
//        bool has_sprite = false;
//        bool has_pose = false;
//
//        for (const auto& comp_handle : obj.get_component_handles()) {
//            auto& comp = factory::component().get_object(comp_handle);
//
//            auto sp_ptr = std::get_if<Sprite>(comp);
//            if (sp_ptr != nullptr) has_sprite = true;
//            auto pose_ptr = std::get_if<Pose>(comp);
//            if (pose_ptr != nullptr) has_pose = true;
//        }
//
//        if (has_sprite && has_pose) {
//            add_component_handle(h);
//        }
//    } else {
//        std::cout << "Tried to access handle of wrong variant" << std::endl;
//    }

}
