//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_OBJECTFACTORY_H
#define ANTNET_OBJECTFACTORY_H

#include <utility>
#include <variant>
#include <vector>
#include <optional>

#include <AntNet/GameObject.h>
#include <AntNet/Handle.h>

template <class FactoryT>
class Factory {
public:
    static Factory& get() {
        static Factory instance;
        return instance;
    }

    Factory(const Factory& o) = delete;
    void operator=(const Factory& o) = delete;

    template <class T, class... Args>
    auto add(Args&&... args) {
        auto comp = T(std::forward<Args>(args)...);
        auto free_handle = find_free_handle();
        if (free_handle.is_valid()) {
            get_object(free_handle) = comp;
            count++;
        }
        return free_handle; // return handle
    }

    FactoryT& get_object(Handle h) {
        return objects[h.get()];
    }

    auto find_free_handle() {
        Handle handle;

        if (objects.size() == count) {
            objects.push_back(std::monostate());
            handle.set(objects.size() - 1);

            return handle;
        }

        // there should be an empty slot, capacity isn't full
        for (std::size_t i = 0; i < objects.size(); ++i) {
            auto e_ptr = std::get_if<std::monostate>(&objects[i]);
            if (e_ptr != nullptr) {
                handle.set(i);
                return handle;
            }
        }

        // uninitialized handle gets returned
        return handle;
    }

private:
    Factory() : count(0) { }; // no one else can instantiate

    std::vector<FactoryT> objects;
    std::size_t count;
};

namespace factory {
    static auto& gameobject() { return Factory<gameobject_t>::get(); }
    static auto& component() { return Factory<component_t>::get(); }
}

#endif //ANTNET_ENGINE_H
