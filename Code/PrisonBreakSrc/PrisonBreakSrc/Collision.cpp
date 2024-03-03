#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y) {

		return true;

	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider)) {
		std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else {
		return false;
	}
}

int Collision::CollisionDirection(const SDL_Rect& recA, const SDL_Rect& recB) {

	std::cout << "RecA: (" << recA.x << ", " << recA.y << ") RecB: (" << recB.x << ", " << recB.y << ")" << std::endl;
	std::cout << "Diff X: " << recB.x - recA.x << " Diff Y: " << recB.y - recA.y << std::endl;


	//when A x or y is less than B x or y


	if ((recA.x < recB.x) && (recB.x - recA.x) > (recB.y - recA.y) && abs(recA.y - recB.y) <= 50) {
		std::cout << "ColType1"<< std::endl;
		return 0; //horizontal
	}
	else if ((recA.y < recB.y) && (recB.x - recA.x) < (recB.y - recA.y) && abs(recA.x - recB.x) <= 50) {
		std::cout << "ColType2" << std::endl;
		return 1; //vertical
	}
	else if ((recA.x - recB.x) > (recA.y - recB.y) && abs(recA.y - recB.y) <= 32) {
		std::cout << "ColType3" << std::endl;
		return 0; //horizontal
	}
	else if ((recA.x - recB.x) < (recA.y - recB.y) && abs(recA.x - recB.x) <= 32) {
		std::cout << "ColType4" << std::endl;
		return 1; //vertical
	}
	
	else {
		return -1; //else
	}
}
