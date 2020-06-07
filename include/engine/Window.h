#pragma once

#include "SDL.h"
#include "engine/structures/Triangle.h"

#include <string>

class Window {
private:
	int width;
	int height;
	SDL_Renderer *renderer;
	void clear();
	void fill_flat_top_triangle(Vector3 &bottom, Vector3 &top_1, Vector3 &top_2);
	void fill_flat_bottom_triangle(Vector3 &top, Vector3 &bottom_1, Vector3 &bottom_2);
public:
	Window(std::string name, int width, int height);
	void update();
	void draw_triangle(Triangle &triangle);
	void fill_triangle(Triangle &triangle);
	int get_width();
	int get_height();
};