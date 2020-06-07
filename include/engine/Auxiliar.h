#pragma once

#include "engine/structures/Vector3.h"
#include "engine/structures/Matrix4x4.h"

class Auxiliar {
public:
	static void multiply_matrix_vector(Vector3 &input, Vector3 &output, Matrix4x4 &matrix);
	static void normalize_vector(Vector3 &vector);
	static float dot_product(Vector3 &vector_1, Vector3 &vector_2);
};