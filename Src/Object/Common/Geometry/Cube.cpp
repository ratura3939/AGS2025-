#include<algorithm>
#include"../../../Utility/Utility.h"
#include "Cube.h"

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _min, const VECTOR& _max)
	: Geometry(_pos,_rot)
{
	UpdateObbAxis();
}

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _squareSize)
	: Geometry(_pos, _rot)
{
	UpdateObbAxis();
}

Cube::~Cube(void)
{
}

const bool Cube::IsHit(Geometry& _geo)
{
	UpdateObbAxis();
	return false;
}

const bool Cube::IsHit(Sphere& _sphere)
{
	return false;
}

const bool Cube::IsHit(Capsule& _capsule)
{
	return false;
}

const bool Cube::IsHit(Cube& _capsule)
{
	return false;
}

const bool Cube::IsHit(Model& _model)
{
	return false;
}

void Cube::DebugDraw(void)
{
}

void Cube::UpdateObbAxis(void)
{
	MATRIX rotMat;
	rotMat = colRot_.ToMatrix();

	obb_.axis[0] = VTransform(VGet(1, 0, 0), rotMat); // Right
	obb_.axis[1] = VTransform(VGet(0, 1, 0), rotMat); // Up
	obb_.axis[2] = VTransform(VGet(0, 0, 1), rotMat); // Forward
}

const VECTOR& Cube::GetAxis(const CUBE_AXIS& _order) const
{
	if (_order == CUBE_AXIS::MAX)return { 0.0f,0.0f,0.0f };
	return obb_.axis[static_cast<int>(_order)];
}

const float Cube::ClosestPointDiff(const VECTOR& _startPos, const VECTOR& _endPos)
{
	// 線分とAABBの最短距離²を求める
		// → 各軸でクランプを行う

	const VECTOR obbMinPos = GetCubeMinPos();
	const VECTOR obbMaxPos = GetCubeMaxPos();

	float t = 0.0f;
	float minDiff = FLT_MAX;

	// 線分上の点 P(t) = A + t*(B - A), 0 <= t <= 1
	const int steps = 10;
	for (int i = 0; i <= steps; ++i)
	{
		float ft = static_cast<float>(i) / steps;
		VECTOR point = VAdd(_startPos, VScale(VSub(_endPos, _startPos), ft));

		// AABB内の最近接点
		VECTOR clamped = {
			std::max(obbMinPos.x, std::min(point.x, obbMaxPos.x)),
			std::max(obbMinPos.y, std::min(point.y, obbMaxPos.y)),
			std::max(obbMinPos.z, std::min(point.z, obbMaxPos.z))
		};

		float distance = Utility::SqrMagnitudeF(VSub(point, clamped));
		if (distance < minDiff)
		{
			minDiff = distance;
			t = ft;
		}
	}

	return minDiff;
}

const VECTOR Cube::GetCubeMinPos(void)
{
	return { obb_.centerPos.x - obb_.halfDiff.x,obb_.centerPos.y - obb_.halfDiff.y,obb_.centerPos.z - obb_.halfDiff.z };
}

const VECTOR Cube::GetCubeMaxPos(void)
{
	return { obb_.centerPos.x + obb_.halfDiff.x,obb_.centerPos.y + obb_.halfDiff.y,obb_.centerPos.z + obb_.halfDiff.z };
}
