#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include "Entity.hpp"
#include "Components.hpp"

class Registry {
    public:
        Entity create() {
            if (!recycled_ids.empty()){
                Entity new_entity = recycled_ids.back();
                recycled_ids.pop_back();
                return new_entity;
            }
            max_entity++;
            return max_entity;
        }

        Entity maxEntity(){
            return max_entity;
        }

        template <typename C>
        std::unordered_map<Entity, C>& getComponent();

        template<>
        std::unordered_map<Entity, Sprite>& getComponent<Sprite>() {
            return sprites;
        }

        template<>
        std::unordered_map<Entity, Transform>& getComponent<Transform>() {
            return transforms;
        }

        template<>
        std::unordered_map<Entity, Input>& getComponent<Input>() {
            return inputs;
        }

        template<>
        std::unordered_map<Entity, PlayerTag>& getComponent<PlayerTag>() {
            return players;
        }

        template<>
        std::unordered_map<Entity, CursorTag>& getComponent<CursorTag>() {
            return cursors;
        }

    private:
        std::vector <uint32_t> recycled_ids;
        uint32_t max_entity = 0;
        std::unordered_map<Entity, Sprite> sprites;
        std::unordered_map<Entity, Transform> transforms;
        std::unordered_map<Entity, Input> inputs;
        std::unordered_map<Entity, PlayerTag> players;
        std::unordered_map<Entity, CursorTag> cursors;
};