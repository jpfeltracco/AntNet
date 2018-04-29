//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_ECS_H
#define ANTNET_ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t max_components = 32;

using ComponentBitSet = std::bitset<max_components>;
using ComponentArray = std::array<Component*, max_components>;

class Component {
public:
    Entity* entity; // reference to owner

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity
{
private:
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray component_array;
    ComponentBitSet component_bit_set;
public:
    void update() {
        for (auto& c : components) c->update();
    }

    void draw() {
        for (auto& c : components) c->draw();
    }
    bool is_active() { return active; }
    void destroy() { active = false; }

    template <typename T>
    bool has_component() const {
        return component_bit_set[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& add_component(TArgs&&... mArgs) {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        component_array[getComponentTypeID<T>()] = c;
        component_bit_set[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T& get_component() const {
        auto ptr = component_array[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);
    }
};

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
public:
    void update() {
        for (auto& e : entities) e->update();
    }

    void draw() {
        for (auto& e : entities) e->draw();
    }

    void refresh() {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity> &mEntity) {
                return !mEntity->is_active();
            }), std::end(entities));
    }

    Entity& add_entity() {
        std::unique_ptr<Entity> uPtr(new Entity());
        Entity& e = *uPtr;
        entities.emplace_back(std::move(uPtr));
        return e;
    }
};

#endif //ANTNET_ECS_H