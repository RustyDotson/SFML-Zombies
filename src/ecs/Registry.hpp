#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <iostream>
#include <functional>
#include <cstdint>

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
            if (!freeList.empty()) {
                Entity entity = freeList.back();
                freeList.pop_back();
                return entity;
            }
            return nextEntity++;
        }

        void destroy(Entity entity){
            freeList.push_back(entity);
        }

        template<typename T>
        void addComponent(Entity entity, const T& component) {
            getStorage<T>().add(entity, component);
        }

        template<typename T>
        T& getComponent(Entity entity) {
            return getStorage<T>().get(entity);
        }

        template<typename T>
        bool hasComponent(Entity entity) {
            return getStorage<T>().has(entity);
        }

        template<typename A, typename B>
        void ForEach(std::function<void(Entity, A&, B&)> func) {
            auto& storageA = getStorage<A>();
            auto& storageB = getStorage<B>();

            // Loop through every entity that has A. If it also has B, call the func.
            for (auto& [entity, compA] : storageA.getEntities()) {
                if (storageB.has(entity)) {
                    func(entity, compA, storageB.get(entity));
                }
            }
        }

        template<typename A>
        void ForEach(std::function<void(Entity, A&)> func) {
            auto& storageA = getStorage<A>();

            // Loop through every entity that has A. If it also has B, call the func.
            for (auto& [entity, compA] : storageA.getEntities()) {
                    func(entity, compA, storageA.get(entity));
            }
        }

        void printRegistry() {
            // This is just for debugging, it prints the available entities in the free list
            std::cout << "Available entities: ";
            for (const auto& entity : freeList) {
                std::cout << entity << " ";
            }
            std::cout << std::endl;
        }
    
    private:
        Entity nextEntity = 0;
        //freeList should be a linked list, just using a vector for now.
        std::vector<Entity> freeList; 

        std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> componentStorages;

        template<typename T>
        ComponentStorage<T>& getStorage() {
            std::type_index type = std::type_index(typeid(T));

            if (!componentStorages.count(type)) {
                componentStorages[type] = std::make_unique<ComponentStorage<T>>();
            }
            return *static_cast<ComponentStorage<T>*>(componentStorages[type].get());
        }
};