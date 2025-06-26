#include "GridSphere.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

void GridSphere::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Matrix* matrix = new Matrix;
	const uint32_t kSubdivision = 10; // 分割数
	const float kLonEvery = float(M_PI) * 2.0f / float(kSubdivision); // 経度分割1つ分の角度
	const float kLatEvery = float(M_PI) / float(kSubdivision); // 緯度分割1つ分の角度
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度
		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度
			// world座標系でのabcを求める
			Vector3 a, b, c;
			a = { sphere.center.x + sphere.radius * cosf(lat) * cosf(lon), sphere.center.y + sphere.radius * sinf(lat), sphere.center.z + sphere.radius * cosf(lat) * sinf(lon) };
			b = { sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon), sphere.center.y + sphere.radius * sinf(lat + kLatEvery), sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon) };
			c = { sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery), sphere.center.y + sphere.radius * sinf(lat), sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery) };
			// screen座標系まで変換
			Vector3 aNdc = matrix->Transform(a, viewProjectionMatrix);
			Vector3 bNdc = matrix->Transform(b, viewProjectionMatrix);
			Vector3 cNdc = matrix->Transform(c, viewProjectionMatrix);
			Vector3 aScreen = matrix->Transform(aNdc, viewportMatrix);
			Vector3 bScreen = matrix->Transform(bNdc, viewportMatrix);
			Vector3 cScreen = matrix->Transform(cNdc, viewportMatrix);
			// ab,acで線を引く
			Novice::DrawLine(static_cast<int>(aScreen.x), static_cast<int>(aScreen.y), static_cast<int>(bScreen.x), static_cast<int>(bScreen.y), color);
			Novice::DrawLine(static_cast<int>(aScreen.x), static_cast<int>(aScreen.y), static_cast<int>(cScreen.x), static_cast<int>(cScreen.y), color);
		}
	}
}

void GridSphere::DrawGrid(Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix)
{
	Matrix* matrix = new Matrix;
	Vector3 startLine = {};
	Vector3 screenStart = {};
	Vector3 endLine = {};
	Vector3 screenEnd = {};
	const float kGridHalfWidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		startLine = { -kGridHalfWidth + kGridEvery * xIndex, 0.0f, -kGridHalfWidth };
		endLine = { -kGridHalfWidth + kGridEvery * xIndex, 0.0f, kGridHalfWidth };
		Vector3 startNdc = matrix->Transform(startLine, viewProjectionMatrix);
		screenStart = matrix->Transform(startNdc, viewportMatrix);
		Vector3 endNdc = matrix->Transform(endLine, viewProjectionMatrix);
		screenEnd = matrix->Transform(endNdc, viewportMatrix);
		Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y), static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
	}
	// 左から右に
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		startLine = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * zIndex };
		endLine = { kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * zIndex };
		Vector3 startNdc = matrix->Transform(startLine, viewProjectionMatrix);
		screenStart = matrix->Transform(startNdc, viewportMatrix);
		Vector3 endNdc = matrix->Transform(endLine, viewProjectionMatrix);
		screenEnd = matrix->Transform(endNdc, viewportMatrix);
		Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y), static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
	}
}
