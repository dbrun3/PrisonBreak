#pragma once

#include "SDL.h"
//#include <SDL2/SDL.h> // mac compatible, todo: test of ok for windows
#include "SDL_image.h"
#include "ECS.h"
#include "MapManager.h"
#include "RoomManager.h"
#include <stdio.h>
#include <iostream>
#include <vector>

#define TILE_SIZE 64

class ColliderComponent; //fwd decl for colliders vector

class Game
{
public:
	Game();
	~Game();

	//initializes game window
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update();
	bool running() { return isRunning; }
    bool forceClosed() { return forceQuit; }
	void render();
	void clean();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;

	//Demo groupings, not set in stone, subject to change
	enum groupLabels : std::size_t {
		groupPlayers,
		groupNPCs,
		groupTiles,		//<--These are the tiles of the current room being rendered
		groupColliders, //<--These are colliders of the current room being rendered
		groupObjects    //<--These are all the objects belonging to the current room being rendered
	};

	struct Doors {
		Entity* north;
		Entity* east;
		Entity* south;
		Entity* west;
	};

private:
	bool isRunning;
    bool forceQuit;
	SDL_Window* window;
    MapManager map;
    RoomManager roomManager;
    Entity* player;
    Entity* metaldoor;
    Entity* idscan;
    Entity* escapeWall;
    Entity* boomWall;
    Entity* barrels;
    Entity* rope_obs;
    Room* currentRoom;
    Navmesh* currentNavmesh;
    std::vector<Entity*>* tiles;
    std::vector<Entity*>* players;
    std::vector<Entity*>* npcs;
    std::vector<Entity*>* colliders;
    std::vector<Entity*>* objects;
};
