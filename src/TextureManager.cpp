//
// Created by jpfeltracco on 4/28/18.
//

#include <iostream>

#include <AntNet/TextureManager.h>
#include <AntNet/Game.h>

SDL_Texture* TextureManager::load_texture(const char* filename) {
    SDL_Surface* temp_surface = IMG_Load(filename);
    if (!temp_surface) std::cout << "Couldn't find texture: " << filename << std::endl;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, temp_surface);
    SDL_FreeSurface(temp_surface);
    return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst) {
    SDL_RenderCopy(Game::renderer, tex, &src, &dst);
}
