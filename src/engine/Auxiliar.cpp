#include "engine/Auxiliar.h"

#include <math.h>

void Auxiliar::multiply_matrix_vector(Vector3 &i, Vector3 &o, Matrix4x4 &m) {
	o.x = i.x * m.values[0][0] + i.y * m.values[1][0] + i.z * m.values[2][0] + m.values[3][0];
	o.y = i.x * m.values[0][1] + i.y * m.values[1][1] + i.z * m.values[2][1] + m.values[3][1];
	o.z = i.x * m.values[0][2] + i.y * m.values[1][2] + i.z * m.values[2][2] + m.values[3][2];
	float w = i.x * m.values[0][3] + i.y * m.values[1][3] + i.z * m.values[2][3] + m.values[3][3];
	
	if (w != 0) {
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
}

void Auxiliar::normalize_vector(Vector3 &vector) {
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
}

float Auxiliar::dot_product(Vector3 &vector_1, Vector3 &vector_2) {
	return vector_1.x * vector_2.x + vector_1.y * vector_2.y + vector_1.z * vector_2.z;
}