//
// Created by jpfeltracco on 4/28/18.
//

#include <iostream>
#include <SDL2/SDL_image.h>

#include <AntNet/Game.h>
#include <AntNet/TextureManager.h>
#include <AntNet/GameObject.h>
#include <AntNet/Map.h>
#include <AntNet/ECS.h>
#include <AntNet/Components.h>

Map* map;
GameObject* player;
GameObject* enemy;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& new_player(manager.add_entity());

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
    player = new GameObject("ant.png", 0, 0);
    enemy = new GameObject("ant_enemy.png", 50, 50);
    map = new Map();

    new_player.add_component<PositionComponent>();
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
    player->update();
    enemy->update();

    manager.update();
    std::cout << new_player.get_component<PositionComponent>().x() << ","
              << new_player.get_component<PositionComponent>().y() << std::endl;
}

void Game::render() {
    SDL_RenderClear(Game::renderer);
    map->draw_map();
    player->render();
    enemy->render();
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