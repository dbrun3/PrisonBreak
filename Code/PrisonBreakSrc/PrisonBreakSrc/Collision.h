#pragma once
#include <SDL.h>
#include "ColliderComponent.h"

class Collision {

public:
	//Returns true if the rects A and B overlap with each other
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);

	//Returns true if two ColliderComponents overlap with each other
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	//Returns 0 for a horizontal collision and 1 for a vertical collision 
	static int CollisionDirection(const SDL_Rect& recA, const SDL_Rect& recB);
};