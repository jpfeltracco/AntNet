//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_OBJECTFACTORY_H
#define ANTNET_OBJECTFACTORY_H

#include <utility>
#include <variant>
#include <vector>
#include <optional>
#include <algorithm>

#include <AntNet/Engine.h>
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

        auto ret_handle = allocate_handle<T>(free_handle, comp);

        return ret_handle;
    }

    FactoryT& get_object(Handle h) {
        return objects[h.get()];
    }

    template <class T>
    Handle allocate_handle(Handle h, T obj) {
        if (h.is_valid()) {
            get_object(h) = obj;
            count++;
            object_handles.push_back(h);
            handle_change(h, false);
            return h;
        }
        return Handle();
    }

    void deallocate_handle(Handle h) {
        get_object(h) = std::monostate();

        if (h.is_valid()) {
            auto h_ptr = std::find(object_handles.begin(), object_handles.end(), h);
            if (h_ptr != object_handles.end()) {
                // remove from vector of active handles
                object_handles.erase(h_ptr);
                handle_change(h, true);
            }
        }
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

    void handle_change(Handle h, bool removed) {
        for (auto recv : handle_change_receivers) {
            recv->notify(h, removed);
        }
    }

    void add_handle_change_receiver(HandleChangeReceiver* rec) {
        handle_change_receivers.push_back(rec);
    }

    const auto& get_object_handles() {
        return object_handles;
    }

private:
    Factory() : count(0) { }; // no one else can instantiate

    std::vector<FactoryT> objects;
    std::vector<Handle> object_handles;
    std::vector<HandleChangeReceiver*> handle_change_receivers;
    std::size_t count;
};

namespace factory {
    static auto& gameobject() { return Factory<gameobject_t>::get(); }
    static auto& component() { return Factory<component_t>::get(); }
}

#endif //ANTNET_ENGINE_H
