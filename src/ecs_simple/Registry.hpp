#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include "Entity.hpp"
#include "Components.hpp"

class Registry {
    public:
        Entity create();

        void destroy(Entity entity);

        Entity maxEntity();

        template <typename C>
        std::unordered_map<Entity, C>& getComponent();

        template<>
        std::unordered_map<Entity, Sprite>& getComponent<Sprite>();

        template<>
        std::unordered_map<Entity, Transform>& getComponent<Transform>();

        template<>
        std::unordered_map<Entity, Input>& getComponent<Input>();

        template<>
        std::unordered_map<Entity, PlayerTag>& getComponent<PlayerTag>();

        template<>
        std::unordered_map<Entity, CursorTag>& getComponent<CursorTag>();

        template<>
        std::unordered_map<Entity, BulletTag>& getComponent<BulletTag>();

        template<>
        std::unordered_map<Entity, AsteroidTag>& getComponent<AsteroidTag>();

    private:
        std::vector <uint32_t> recycled_ids;
        uint32_t max_entity = 0;
        std::unordered_map<Entity, Sprite> sprites;
        std::unordered_map<Entity, Transform> transforms;
        std::unordered_map<Entity, Input> inputs;
        std::unordered_map<Entity, PlayerTag> players;
        std::unordered_map<Entity, CursorTag> cursors;
        std::unordered_map<Entity, BulletTag> bullets;
        std::unordered_map<Entity, AsteroidTag> asteroids;
};