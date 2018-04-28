//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_OBJECTFACTORY_H
#define ANTNET_OBJECTFACTORY_H

#include <utility>
#include <variant>
#include <vector>

#include "GameObject.h"
#include "Component.h"

class ObjectFactory {
public:
    static ObjectFactory& get() {
        static ObjectFactory instance;
        return instance;
    }
    ObjectFactory(ObjectFactory const& o) = delete;
    void operator=(ObjectFactory const& o) = delete;

    template <class T, class... Args>
    auto add_component(Args&&... args) {
        auto comp = T(std::forward<Args>(args)...);
        components.push_back(comp);
        return static_cast<int>(components.size()) - 1; // return handle
    }

    template <class T, class... Args>
    auto add_game_object(Args&&... args) {
        auto comp = T(std::forward<Args>(args)...);
        gameobjects.push_back(comp);
        return static_cast<int>(gameobjects.size()) - 1; // return handle
    }

private:
    ObjectFactory() = default; // no one else can instantiate
    std::vector<gameobject_t> gameobjects;
    std::vector<component_t> components;
};


#endif //ANTNET_ENGINE_H
