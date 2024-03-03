#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

/* Entity Component System (ECS) 
* 
* The ECS is how we will be creating and managing Game Objects
* AKA Entities. Entities can be created by adding them to the 
* entity manager via the call
* 
* auto& entityName(manager.addEntity());

* Entities are drawn and updated by the manager class
* defined below, but most importantly possess Components.
* Components are properties that an entity may possess and are added
* to entities via the call:
*
* entityName.addComponent(<ComponentName>(args));
* 
* Through this system, we can easily add common properties to 
* game objects such as having a position (transform) and sprite.
* It also allows us to easily add new properties that we create
* to existing game objects, like giving the player object the
* ability to act upon keyboard input.
* 
*/



class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentArray = std::array<Component*, maxComponents>;
using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxComponents>;

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
private: 
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitset componentBitset;
	GroupBitset groupBitset;

public:
	Entity(Manager& mManager) : manager(mManager) {};

	void update() {
		for (auto& c : components) c->update();
	}

	void draw() {
		for (auto& c : components) c->draw();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup) {
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup) {
		groupBitset[mGroup] = false;
	}

	template <typename T> bool hasComponent() const {
        return componentBitset[getComponentTypeID<T>()];
	}

	//Attaches a new Component of <type> instance to an Entity
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...)); //WTF is this line lmaooo it works tho ig
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		
		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	//Returns the Component attached to an Entity of a specified type
	template<typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager {
private:

	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
	
	void update() {
		for (auto& e : entities) e->update();
	}

	void draw() {
		for (auto& e : entities) e->draw();
	}

	void refresh() {

		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity) {
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}), std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity) {
				return !mEntity->isActive();
			}),
				std::end(entities));
	}
    
    void reset() {
        for (auto i(0u); i < maxGroups; i++) {
            auto& v(groupedEntities[i]);
            v.clear();
        }
        entities.clear();
    }

	void AddToGroup(Entity* mEntity, Group mGroup) {
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup) {
		return groupedEntities[mGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		
		return *e;
	}
};


