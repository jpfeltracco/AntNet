//
// Created by jpfeltracco on 4/29/18.
//

#ifndef ANTNET_COLLIDERCOMPONENT_H
#define ANTNET_COLLIDERCOMPONENT_H

#include <string>

#include <SDL2/SDL.h>

#include <AntNet/Game.h>
#include <AntNet/ECS/ECS.h>
#include <AntNet/ECS/Components.h>


class ColliderComponent : public Component {
public:
    SDL_Rect collider;
    std::string tag;

    TransformComponent* transform;

    ColliderComponent() = default;
    ColliderComponent(std::string tag) : tag(tag) {}

    void init() override {
        if (!entity->has_component<TransformComponent>()) {
            entity->add_component<TransformComponent>();
        }
        transform = &entity->get_component<TransformComponent>();

        Game::colliders.push_back(this);
    }

    void update() override {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }

};

#endif //ANTNET_COLLIDERCOMPONENT_H
