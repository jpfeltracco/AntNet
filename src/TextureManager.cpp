//
// Created by jpfeltracco on 4/28/18.
//

#include <AntNet/TextureManager.h>

SDL_Texture* TextureManager::load_texture(const char* filename, SDL_Renderer* ren) {
    SDL_Surface* temp_surface = IMG_Load(filename);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, temp_surface);
    SDL_FreeSurface(temp_surface);
    return tex;
}
