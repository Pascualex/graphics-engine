#include "engine/Engine.h"
#include "engine/Auxiliar.h"

#include <iostream>
#include <thread>

Engine::Engine(std::string name, int width, int height, int fps):
window(name, width, height) {
	if (width <= 0) throw std::invalid_argument("Engine (constructor): width must be higher that zero");
	if (height <= 0) throw std::invalid_argument("Engine (constructor): height must be higher that zero");

	using namespace std::chrono_literals;
	step_duration = 1000000us / fps;
	current_step = 0;
	next_step_time = std::chrono::system_clock::now();
	stop_called = false;

	camera_position = { 0.0f, 0.0f, 0.0f };
	theta = 0;

	// Projection matrix

	float z_near = 0.1f;
	float z_far = 1000.0f;
	float fov = 90.0f;
	float aspect_ratio = (float)height / (float)width;
	float fov_rad = 1.0f / tan(fov * 0.5f / 180.0f * M_PI);
	float z_normalization = z_far / (z_far - z_near);

	projection_matrix.values[0][0] = aspect_ratio * fov_rad;
	projection_matrix.values[1][1] = fov_rad;
	projection_matrix.values[2][2] = z_normalization;
	projection_matrix.values[3][2] = -z_near * z_normalization;
	projection_matrix.values[2][3] = 1.0f;

	// TEMP
	cube_mesh.triangles = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }
	};
}

void Engine::start() {
	for (current_step = 0; current_step < 10000; current_step++) {
		if (stop_called) {
			std::cout << std::endl;
			std::cout << "The engine received an interruption and successfully stopped" << std::endl;
			return;
		}

		update();

		next_step_time += step_duration;
		if (next_step_time > std::chrono::system_clock::now()) {
			std::this_thread::sleep_until(next_step_time);
		} else {
			std::cout << "Falling back on FPS" << std::endl;
		}
	}

	std::cout << std::endl;
	std::cout << "The engine reached the maximum step" << std::endl;
}

void Engine::update() {
	theta += 0.03f;

	// Roration X
	Matrix4x4 rotation_x_matrix;
	rotation_x_matrix.values[0][0] = 1;
	rotation_x_matrix.values[1][1] = cosf(theta * 0.5f);
	rotation_x_matrix.values[1][2] = sinf(theta * 0.5f);
	rotation_x_matrix.values[2][1] = -sinf(theta * 0.5f);
	rotation_x_matrix.values[2][2] = cosf(theta * 0.5f);;
	rotation_x_matrix.values[3][3] = 1;

	// Rotation Z
	Matrix4x4 rotation_z_matrix;
	rotation_z_matrix.values[0][0] = cosf(theta);
	rotation_z_matrix.values[0][1] = sinf(theta);
	rotation_z_matrix.values[1][0] = -sinf(theta);
	rotation_z_matrix.values[1][1] = cosf(theta);
	rotation_z_matrix.values[2][2] = 1;
	rotation_z_matrix.values[3][3] = 1;

	for (Triangle triangle : cube_mesh.triangles) {
		Triangle rot_z_triangle;

		Auxiliar::multiply_matrix_vector(triangle.points[0], rot_z_triangle.points[0], rotation_z_matrix);
		Auxiliar::multiply_matrix_vector(triangle.points[1], rot_z_triangle.points[1], rotation_z_matrix);
		Auxiliar::multiply_matrix_vector(triangle.points[2], rot_z_triangle.points[2], rotation_z_matrix);

		Triangle rot_x_triangle;

		Auxiliar::multiply_matrix_vector(rot_z_triangle.points[0], rot_x_triangle.points[0], rotation_x_matrix);
		Auxiliar::multiply_matrix_vector(rot_z_triangle.points[1], rot_x_triangle.points[1], rotation_x_matrix);
		Auxiliar::multiply_matrix_vector(rot_z_triangle.points[2], rot_x_triangle.points[2], rotation_x_matrix);

		Triangle trans_triangle = rot_x_triangle;

		trans_triangle.points[0].z += 3.0f;
		trans_triangle.points[1].z += 3.0f;
		trans_triangle.points[2].z += 3.0f;

		Vector3 normal, line_1, line_2;

		line_1.x = trans_triangle.points[1].x - trans_triangle.points[0].x;
		line_1.y = trans_triangle.points[1].y - trans_triangle.points[0].y;
		line_1.z = trans_triangle.points[1].z - trans_triangle.points[0].z;

		line_2.x = trans_triangle.points[2].x - trans_triangle.points[0].x;
		line_2.y = trans_triangle.points[2].y - trans_triangle.points[0].y;
		line_2.z = trans_triangle.points[2].z - trans_triangle.points[0].z;

		normal.x = line_1.y * line_2.z - line_1.z * line_2.y;
		normal.y = line_1.z * line_2.x - line_1.x * line_2.z;
		normal.z = line_1.x * line_2.y - line_1.y * line_2.x;

		Vector3 camera_line;

		camera_line.x = trans_triangle.points[0].x - camera_position.x;
		camera_line.y = trans_triangle.points[0].y - camera_position.y;
		camera_line.z = trans_triangle.points[0].z - camera_position.z;

		float dot_camera = Auxiliar::dot_product(normal, camera_line);

		if (dot_camera >= 0) continue;

		Triangle projected_triangle;

		Auxiliar::multiply_matrix_vector(trans_triangle.points[0], projected_triangle.points[0], projection_matrix);
		Auxiliar::multiply_matrix_vector(trans_triangle.points[1], projected_triangle.points[1], projection_matrix);
		Auxiliar::multiply_matrix_vector(trans_triangle.points[2], projected_triangle.points[2], projection_matrix);

		projected_triangle.points[0].x += 1.0f;
		projected_triangle.points[0].y += 1.0f;
		projected_triangle.points[1].x += 1.0f;
		projected_triangle.points[1].y += 1.0f;
		projected_triangle.points[2].x += 1.0f;
		projected_triangle.points[2].y += 1.0f;

		projected_triangle.points[0].x *= 0.5f * (float) window.get_width();
		projected_triangle.points[0].y *= 0.5f * (float) window.get_height();
		projected_triangle.points[1].x *= 0.5f * (float) window.get_width();
		projected_triangle.points[1].y *= 0.5f * (float) window.get_height();
		projected_triangle.points[2].x *= 0.5f * (float) window.get_width();
		projected_triangle.points[2].y *= 0.5f * (float) window.get_height();

		window.fill_triangle(projected_triangle);
		window.draw_triangle(projected_triangle);
	}

	window.update();
}

void Engine::stop() {
	stop_called = true;
}