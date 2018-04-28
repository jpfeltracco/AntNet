//
// Created by jpfeltracco on 4/27/18.
//

#include <AntNet/GameObject.h>
#include <AntNet/ObjectFactory.h>

GameObject::GameObject() {
}

void GameObject::update() {

}

void GameObject::add_component(Handle comp_handle) {
    components.push_back(comp_handle);
}

Tile::Tile() {
    auto pose_handle = factory::component().add<Pose>();
    auto sprite_handle = factory::component().add<Sprite>();

    add_component(pose_handle);
    add_component(sprite_handle);
}

Tile::Tile(int x, int y, int texture_handle) {
    auto pose_handle = factory::component().add<Pose>(x, y, Pose::Direction::North);
    auto sprite_handle = factory::component().add<Sprite>(texture_handle);

    add_component(pose_handle);
    add_component(sprite_handle);
}
