//
// Created by jpfeltracco on 4/29/18.
//

#ifndef ANTNET_COLLISION_H
#define ANTNET_COLLISION_H

#include <SDL2/SDL.h>

class ColliderComponent;

class Collision {
public:
    static bool AABB(const SDL_Rect& rec_a, const SDL_Rect& rec_b);

    static bool AABB(const ColliderComponent& collider_a, const ColliderComponent& collider_b);
};

#endif //ANTNET_COLLISION_H
