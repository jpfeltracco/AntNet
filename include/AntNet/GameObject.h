//
// Created by jpfeltracco on 4/27/18.
//

#ifndef ANTNET_GAMEOBJECT_H
#define ANTNET_GAMEOBJECT_H

#include <variant>
#include <vector>

#include <AntNet/ECS/Components.h>
#include <AntNet/Handle.h>
#include <AntNet/Game.h>

class GameObject {
public:
    GameObject(const char* texturesheet, int x, int y);
    ~GameObject();

    void update();
    void render();

private:
    int xpos;
    int ypos;

    SDL_Texture* obj_texture;
    SDL_Rect src_rect, dst_rect;
};


#endif //ANTNET_GAMEOBJECT_H
