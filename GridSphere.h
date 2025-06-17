#pragma once
#include "Matrix.h"
#include <cmath>

struct Sphere {
	Vector3 center; // 中心点
	float radius; // 半径
};

class GridSphere
{
public:
	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
	void DrawGrid(Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix);
};

