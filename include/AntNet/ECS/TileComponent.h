//
// Created by jpfeltracco on 4/29/18.
//

#ifndef ANTNET_TILECOMPONENT_H
#define ANTNET_TILECOMPONENT_H

#include <AntNet/ECS/ECS.h>
#include <AntNet/ECS/TransformComponent.h>
#include <AntNet/ECS/SpriteComponent.h>
#include <SDL2/SDL.h>

class TileComponent : public Component {
public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    SDL_Rect tile_rect;
    int tileID;

    const char* path;

    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, int id)
        : tileID(id)
    {
        tile_rect.x = x;
        tile_rect.y = y;
        tile_rect.w = w;
        tile_rect.h = h;

        switch (tileID) {
            case 0:
                path = "water.png";
                break;
            case 1:
                path = "dirt.png";
                break;
            case 2:
                path = "grass.png";
                break;
            default:
                break;
        }
    }

    void init() override {
        entity->add_component<TransformComponent>(static_cast<float>(tile_rect.x),
                                                  static_cast<float>(tile_rect.y),
                                                  tile_rect.w, tile_rect.h, 1);
        transform = &entity->get_component<TransformComponent>();

        entity->add_component<SpriteComponent>(path);
        sprite = &entity->get_component<SpriteComponent>();
    }
};

#endif //ANTNET_TILECOMPONENT_H
