#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Entity.hpp"


class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;
        virtual void remove(Entity entity) = 0;
};


template<typename T>
class ComponentStorage : public IComponentStorage {
    public:
        void add(Entity entity, const T& component) {
            // adds an entity and its component to the storage, 
            // if the entity already has a component the component will be replaced
            lookup[entity] = components.size();
            entities.push_back(entity);
            components.push_back(component);
        }

        void remove(Entity entity) override {
            // removes an entity and its component from the storage,
            // The final entity/component pair is then moved into the same 
            // index as the removed entity/component pair, 
            // and the lookup is updated to reflect this change

            if (!has(entity)) return;

            size_t index = lookup[entity];
            size_t lastIndex = components.size() - 1;

            components[index] = components[lastIndex];
            entities[index] = entities[lastIndex];
            lookup[entities[index]] = index;
            lookup.erase(entity);
        }

        bool has(Entity entity) const {
            return lookup.find(entity) != lookup.end();
        }

        T& get(Entity entity) {
            return components[lookup[entity]];
        }

        const std::vector<Entity>& getEntities() const {
            return entities;
        }

    private:
        // variables used in a sparce set ds that allows easy 
        // addition and removal of components
        std::vector<T> components;
        std::vector<Entity> entities;
        std::unordered_map<Entity, size_t> lookup;
};


class Registry {
    public:
        Entity create() {
            if (!registeredEntities.empty()) {
                Entity entity = registeredEntities.back();
                registeredEntities.pop_back();
                return entity;
            }
            return nextEntity++;
        }

        void destroy(Entity entity){
            registeredEntities.push_back(entity);
        }
    
    private:
        Entity nextEntity = 0;
        std::vector<Entity> registeredEntities;
};