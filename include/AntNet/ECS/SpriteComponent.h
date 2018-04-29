//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_SPRITECOMPONENT_H
#define ANTNET_SPRITECOMPONENT_H

#include <SDL2/SDL.h>

#include <AntNet/ECS/Components.h>
#include <AntNet/TextureManager.h>

class SpriteComponent : public Component {
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect src_rect, dest_rect;
public:
    SpriteComponent() = default;
    SpriteComponent(const char* path) {
        set_tex(path);
    }

    ~SpriteComponent() {
        SDL_DestroyTexture(texture);
    }

    void set_tex(const char* path) {
        texture = TextureManager::load_texture(path);
    }

    void init() override {
        transform = &entity->get_component<TransformComponent>();

        src_rect.x = src_rect.y  = 0;
        src_rect.w = transform->width;
        src_rect.h = transform->height;
    }

    void update() override {
        dest_rect.x = static_cast<int>(transform->position.x);
        dest_rect.y = static_cast<int>(transform->position.y);
        dest_rect.w = transform->width * transform->scale;
        dest_rect.h = transform->height * transform->scale;
    }

    void draw() override {
        TextureManager::draw(texture, src_rect, dest_rect);
    }
};
#endif //ANTNET_SPRITECOMPONENT_H
