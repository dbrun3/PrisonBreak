#pragma once

struct Animation {
	int index;
	int col;
	int frames;
	int speed;

	Animation() {}

	Animation(int i, int c, int f, int s) {
		index = i; // what row of the spritesheet to use (starts from index 0)
		col = c; // what col to start from
		frames = f; // how many frames of the row to use (how many frames the animation has)
		speed = s; // delay between frames in milliseconds
	}
};