#include <iostream>

#include <SDL2/SDL.h>
#include <AntNet/SDL_wrap.h>

#include <AntNet/Engine.h>
#include <AntNet/GameObject.h>
#include <AntNet/ObjectFactory.h>

constexpr Uint32 fps = 60;
constexpr Uint32 frame_delay = 1000 / fps;
constexpr bool fps_limit = true;

int main(int argc, char* argv[]) {
    Engine e;

    ObjectFactory::get().add_game_object<Tile>(0, 0, Pose::Direction::North, 1);
    ObjectFactory::get().add_game_object<Tile>(1, 0, Pose::Direction::South, 1);
    ObjectFactory::get().add_game_object<Tile>(0, 1, Pose::Direction::West, 1);

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    { // ensure all SDL resources deleted before SDL_Quit
        auto window = sdl2::make_window(
                "An SDL2 window",                  // window title
                SDL_WINDOWPOS_UNDEFINED,           // initial x position
                SDL_WINDOWPOS_UNDEFINED,           // initial y position
                640,                               // width, in pixels
                480,                               // height, in pixels
                SDL_WINDOW_OPENGL);                // flags - see below

        bool quit = false;
        SDL_Event event;

        Uint32 frame_start;
        Uint32 frame_time;
        Uint32 last_time;

        while (!quit) {
            frame_start = SDL_GetTicks();

            // Event handling
            while (SDL_PollEvent(&event)) {
                // handle events
                if (event.type == SDL_QUIT) quit = true;
            }

            // Logic
            float dt = (SDL_GetTicks() - last_time) / 1000.0;
            e.update(dt);

            // Rendering
            auto screenSurface = SDL_GetWindowSurface(window.get());
            SDL_FillRect(screenSurface, nullptr, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            SDL_UpdateWindowSurface(window.get());

            // FPS management
            frame_time = SDL_GetTicks() - frame_start;
            if (fps_limit && frame_delay > frame_time) SDL_Delay(frame_delay - frame_time);
        }
    }

    // Clean up
    SDL_Quit();

    return 0;
}

