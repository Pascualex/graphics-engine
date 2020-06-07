#pragma once

#include "engine/Window.h"
#include "engine/structures/Matrix4x4.h"
#include "engine/structures/Mesh.h"

#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <chrono>

class Engine {
private:
	std::chrono::system_clock::duration step_duration;
	int current_step;
	std::chrono::time_point<std::chrono::system_clock> next_step_time;
	bool stop_called;

	Matrix4x4 projection_matrix;
	Vector3 camera_position;
	float theta;

	Window window;

	Mesh cube_mesh;

	void update();
public:
	Engine(std::string name, int width, int height, int fps);
	void start();
	void stop();
};