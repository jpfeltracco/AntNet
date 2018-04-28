//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_GAME_H
#define ANTNET_GAME_H

#include <SDL2/SDL.h>

class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handle_events();
    void update();
    void render();
    void clean();

    bool running();

private:
    int cnt;
    bool is_running;
    SDL_Window* window;
    SDL_Renderer* renderer;
};


#endif //ANTNET_GAME_H
