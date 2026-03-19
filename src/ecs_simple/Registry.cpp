#include <vector>
#include <iostream>
#include <unordered_map>
#include "Entity.hpp"
#include "Components.hpp"
#include "Registry.hpp"


Entity Registry::create() {
    if (!recycled_ids.empty()){
        Entity new_entity = recycled_ids.back();
        recycled_ids.pop_back();
        return new_entity;
    }
    max_entity++;
    std::cout << "Created entity: " << max_entity << std::endl;
    return max_entity;
}

Entity Registry::maxEntity(){
    return max_entity;
}

template<>
std::unordered_map<Entity, Sprite>& Registry::getComponent<Sprite>() {
    return sprites;
}

template<>
std::unordered_map<Entity, Transform>& Registry::getComponent<Transform>() {
    return transforms;
}

template<>
std::unordered_map<Entity, Input>& Registry::getComponent<Input>() {
    return inputs;
}

template<>
std::unordered_map<Entity, PlayerTag>& Registry::getComponent<PlayerTag>() {
    return players;
}

template<>
std::unordered_map<Entity, CursorTag>& Registry::getComponent<CursorTag>() {
    return cursors;
}

template<>
std::unordered_map<Entity, BulletTag>& Registry::getComponent<BulletTag>() {
    return bullets;
}