//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_TEXTUREMANAGER_H
#define ANTNET_TEXTUREMANAGER_H

#include <SDL2/SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* load_texture(const char* filename);
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst);
};

#endif //ANTNET_TEXTUREMANAGER_H
