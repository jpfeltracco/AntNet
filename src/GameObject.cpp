//
// Created by jpfeltracco on 4/27/18.
//

#include <AntNet/GameObject.h>
#include <AntNet/ObjectFactory.h>

GameObject::GameObject() {
    for (auto& e : components) e = -1; // no handle found
}

void GameObject::update() {

}

int GameObject::get_component_handle(Component::Type type) {
    return components[static_cast<std::size_t>(type)];
}

void GameObject::set_component_handle(Component::Type type, int handle) {
    components[static_cast<std::size_t>(type)] = handle;
}

Tile::Tile() {
    auto pose_handle = ObjectFactory::get().add_component<Pose>();
    this->set_component_handle(Component::Type::Pose, pose_handle);
}

Tile::Tile(int x, int y, Pose::Direction d, int texture_handle) {
    auto pose_handle = ObjectFactory::get().add_component<Pose>(x, y, d);
    this->set_component_handle(Component::Type::Pose, pose_handle);
    auto sprite_handle = ObjectFactory::get().add_component<Sprite>(texture_handle);
    this->set_component_handle(Component::Type::Sprite, sprite_handle);
}
