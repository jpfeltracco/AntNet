#include <iostream>

#include <SDL2/SDL.h>

#include <AntNet/Game.h>

constexpr Uint32 fps = 60;
constexpr Uint32 frame_delay = 1000 / fps;
constexpr bool fps_limit = true;

Game* game = nullptr;

int main(int argc, char* argv[]) {

    const int fps = 60;
    const int frame_delay = 1000/fps;

    Uint32 frame_start;
    Uint32 frame_time;

    game = new Game();

    game->init("AntNet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
               800, 640, false);

    while (game->running()) {
        frame_start = SDL_GetTicks();

        game->handle_events();
        game->update();
        game->render();

        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < frame_delay) SDL_Delay(frame_delay - frame_time);
    }

    game->clean();

    return 0;
}

