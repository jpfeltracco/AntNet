//
// Created by jpfeltracco on 4/29/18.
//

#include <AntNet/Collision.h>
#include <AntNet/ECS/ColliderComponent.h>
#include <AntNet/ECS/ColliderComponent.h>

bool Collision::AABB(const SDL_Rect& rec_a, const SDL_Rect& rec_b) {
    bool collided = rec_a.x + rec_a.w >= rec_b.x &&
                    rec_b.x + rec_b.w >= rec_a.x &&
                    rec_a.y + rec_a.h >= rec_b.y &&
                    rec_b.y + rec_b.h >= rec_a.y;
    return collided;
}

bool Collision::AABB(const ColliderComponent& collider_a, const ColliderComponent& collider_b)
{
    bool collided = Collision::AABB(collider_a.collider, collider_b.collider);

    if (collided) {
        std::cout << collider_a.tag << " hit " << collider_b.tag << std::endl;
    }

    return collided;
}

