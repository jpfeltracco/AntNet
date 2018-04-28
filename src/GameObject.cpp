//
// Created by jpfeltracco on 4/27/18.
//

#include <AntNet/GameObject.h>
#include <AntNet/TextureManager.h>

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y)
    : xpos(x), ypos(y), renderer(ren) {
    obj_texture = TextureManager::load_texture(texturesheet, ren);
}

GameObject::~GameObject() {

}

void GameObject::update() {
    xpos++;
    ypos++;


    src_rect.h = 32;
    src_rect.w = 32;
    src_rect.x = 0;
    src_rect.y = 0;

    constexpr auto scaling_factor = 2;
    dst_rect.h = src_rect.h * scaling_factor;
    dst_rect.w = src_rect.w * scaling_factor;
    dst_rect.x = xpos;
    dst_rect.y = ypos;
}

void GameObject::render() {
    SDL_RenderCopy(renderer, obj_texture, &src_rect, &dst_rect);
}



