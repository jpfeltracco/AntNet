//
// Created by jpfeltracco on 4/28/18.
//

#include <iostream>
#include <SDL2/SDL_image.h>

#include <AntNet/Game.h>
#include <AntNet/TextureManager.h>
#include <AntNet/Map.h>
#include <AntNet/ECS/Components.h>
#include <AntNet/Vector2D.h>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;

const Uint8* Game::keyboard_state = nullptr;

auto& player(manager.add_entity());

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

        Game::renderer = SDL_CreateRenderer(window, -1, 0);

        if (!Game::renderer) {
            std::cout << "Couldn't create renderer" << std::endl;
            is_running = false;
        } else {
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
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
    map = new Map();

    player.add_component<TransformComponent>();
    player.add_component<SpriteComponent>("ant.png");
    player.add_component<KeyboardController>();
}

void Game::handle_events() {
    Game::keyboard_state = SDL_GetKeyboardState(nullptr);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            default:
                break;
        }
    }

}

void Game::update() {
    manager.refresh();
    manager.update();
}

void Game::render() {
    SDL_RenderClear(Game::renderer);
    map->draw_map();
    manager.draw();
    SDL_RenderPresent(Game::renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Game::renderer);

    SDL_Quit();

    std::cout << "Game cleaned" << std::endl;
}

bool Game::running() {
    return is_running;
}