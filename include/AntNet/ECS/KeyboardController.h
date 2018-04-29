//
// Created by jpfeltracco on 4/29/18.
//

#ifndef ANTNET_KEYBOARDCONTROLLER_H
#define ANTNET_KEYBOARDCONTROLLER_H

#include <AntNet/Game.h>
#include <AntNet/ECS/ECS.h>
#include <AntNet/ECS/Components.h>

class KeyboardController : public Component {
public:
    TransformComponent* transform;

    void init() override {
        transform = &entity->get_component<TransformComponent>();
    }

    void update() override {
        transform->velocity.x = 0;
        transform->velocity.y = 0;
        if (Game::keyboard_state[SDL_SCANCODE_W]) {
            transform->velocity.y -= 1;
        }
        if (Game::keyboard_state[SDL_SCANCODE_A]) {
            transform->velocity.x -= 1;
        }
        if (Game::keyboard_state[SDL_SCANCODE_S]) {
            transform->velocity.y += 1;
        }
        if (Game::keyboard_state[SDL_SCANCODE_D]) {
            transform->velocity.x += 1;
        }

    }
};


#endif //ANTNET_KEYBOARDCONTROLLER_H
