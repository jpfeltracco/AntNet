//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_TEXTUREMANAGER_H
#define ANTNET_TEXTUREMANAGER_H

#include <SDL2/SDL_image.h>

class TextureManager {
public:
    static SDL_Texture* load_texture(const char* filename, SDL_Renderer* ren);
};

#endif //ANTNET_TEXTUREMANAGER_H
