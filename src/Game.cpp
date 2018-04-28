//
// Created by jpfeltracco on 4/28/18.
//

#include <iostream>
#include <SDL2/SDL_image.h>

#include <AntNet/Game.h>
#include <AntNet/TextureManager.h>

SDL_Texture* ant_tex;
SDL_Rect src_r, dest_r;

Game::Game() : cnt(0) {

}

Game::~Game() {

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    is_running = true;

    Uint32 flags = 0;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window) {
            std::cout << "Window created" << std::endl;
        } else {
            is_running = false;
        }

        if (!window) {
            is_running = false;
            std::cout << "Couldn't create window" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);

        if (!renderer) {
            std::cout << "Couldn't create renderer" << std::endl;
            is_running = false;
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

    } else {
        is_running = false;
    }

    SDL_Surface* tmp_surface = IMG_Load("ant.png");
    if (tmp_surface == nullptr) {
        std::cout << "Failed to find ant.png" << std::endl;
        is_running = false;
        return;
    }
    ant_tex = TextureManager::load_texture("ant.png", renderer);
}

void Game::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    cnt++;

    dest_r.h = 64;
    dest_r.w = 64;

    dest_r.x = cnt;
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, ant_tex, nullptr, &dest_r);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    std::cout << "Game cleaned" << std::endl;
}

bool Game::running() {
    return is_running;
}