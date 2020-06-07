#include "engine/Window.h";

#include <algorithm>

Window::Window(std::string name, int width, int height) {
	this->width = width;
	this->height = height;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);

	clear();
}

void Window::update() {
	SDL_RenderPresent(renderer);
	clear();
}

void Window::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Window::draw_triangle(Triangle &t) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLineF(renderer, t.points[0].x, t.points[0].y, t.points[1].x, t.points[1].y);
	SDL_RenderDrawLineF(renderer, t.points[1].x, t.points[1].y, t.points[2].x, t.points[2].y);
	SDL_RenderDrawLineF(renderer, t.points[2].x, t.points[2].y, t.points[0].x, t.points[0].y);
}

void Window::fill_triangle(Triangle &t) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	Vector3 points[3] = {
		{ round(t.points[0].x), round(t.points[0].y), round(t.points[0].z) },
		{ round(t.points[1].x), round(t.points[1].y), round(t.points[1].z) },
		{ round(t.points[2].x), round(t.points[2].y), round(t.points[2].z) }
	};

	std::sort(points, points + 3, [](const Vector3 &p_1, const Vector3 &p_2) {
		return p_1.y < p_2.y;
	});

	if (points[0].y == points[1].y) {
		fill_flat_top_triangle(points[2], points[0], points[1]);
	} else if (points[1].y == points[2].y) {
		fill_flat_bottom_triangle(points[0], points[1], points[2]);
	} else {
		float r = (points[1].y - points[0].y) / (points[2].y - points[0].y);
		Vector3 extra_point = { points[0].x + r * (points[2].x - points[0].x), points[1].y, 0 };
		fill_flat_top_triangle(points[2], points[1], extra_point);
		fill_flat_bottom_triangle(points[0], points[1], extra_point);
	}
}

void Window::fill_flat_top_triangle(Vector3 &bottom, Vector3 &top_1, Vector3 &top_2) {
	float slope_1 = (top_1.x - bottom.x) / (top_1.y - bottom.y);
	float slope_2 = (top_2.x - bottom.x) / (top_2.y - bottom.y);

	float x_1 = bottom.x;
	float x_2 = bottom.x;

	for (float y = bottom.y; y >= top_1.y; y--) {
		SDL_RenderDrawLineF(renderer, x_1, y, x_2, y);
		x_1 -= slope_1;
		x_2 -= slope_2;
	}
}

void Window::fill_flat_bottom_triangle(Vector3 &top, Vector3 &bottom_1, Vector3 &bottom_2) {
	float slope_1 = (bottom_1.x - top.x) / (bottom_1.y - top.y);
	float slope_2 = (bottom_2.x - top.x) / (bottom_2.y - top.y);

	float x_1 = top.x;
	float x_2 = top.x;

	for (float y = top.y; y <= bottom_1.y; y++) {
		SDL_RenderDrawLineF(renderer, x_1, y, x_2, y);
		x_1 += slope_1;
		x_2 += slope_2;
	}
}

int Window::get_width() {
	return width;
}

int Window::get_height() {
	return height;
}