//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_MAP_H
#define ANTNET_MAP_H

#include <AntNet/Game.h>


class Map {
public:

    Map();
    ~Map();

    void load_map(int arr[20][25]);
    void draw_map();

private:

    SDL_Rect src, dest;

    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    int map[20][25];
};


#endif //ANTNET_MAP_H
