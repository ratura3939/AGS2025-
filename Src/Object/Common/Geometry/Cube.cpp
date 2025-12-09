#include<algorithm>
#include"../../../Utility/Utility.h"
#include"Sphere.h"
#include"Capsule.h"
#include "Cube.h"

namespace {
	const int NORMAL_COLOR = 0xff00ff;
}

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _halfSize)
	: Geometry(_pos, _rot)
	, testMemoryVec_(Utility::VECTOR_INIT)
{
	obb_.halfDiff = _halfSize;
	UpdateObbAxis();
}

Cube::~Cube(void)
{
}

const bool Cube::IsHit(Geometry& _geo)
{
	UpdateObbAxis();
	return _geo.IsHit(*this);
}

const bool Cube::IsHit(Sphere& _sphere)
{
	return _sphere.IsHit(*this);
}

const bool Cube::IsHit(Capsule& _capsule)
{
	// OBB のワールド中心
	VECTOR worldCenter = colPos_;

	// カプセル線分をOBBのローカル空間に変換
	VECTOR rel1 = VSub(_capsule.GetPosTop(), worldCenter);
	VECTOR rel2 = VSub(_capsule.GetPosBottom(), worldCenter);

	VECTOR local1 = {
		VDot(rel1, obb_.axis[0]),
		VDot(rel1, obb_.axis[1]),
		VDot(rel1, obb_.axis[2])
	};

	VECTOR local2 = {
		VDot(rel2, obb_.axis[0]),
		VDot(rel2, obb_.axis[1]),
		VDot(rel2, obb_.axis[2])
	};

	// スラブ法で最近接点を見つける
	// AABBとして処理する（OBBローカル空間内で）

	float distSq = ClosestPointDiff(local1, local2);

	bool isHit = distSq <= (_capsule.GetRadius() * _capsule.GetRadius());

	if (isHit) {
		const VECTOR localHitPoint = testMemoryVec_;
		const MATRIX rotMat = colRot_.ToMatrix();
		const VECTOR worldHitPosition = VAdd(VTransform(localHitPoint, rotMat), colPos_);
			
		_capsule.SetHitPoint(worldHitPosition);

		//押し戻し方向
		VECTOR nomal = Utility::VNormalize(VTransform(localHitPoint, rotMat));
		_capsule.SetHitNormal(Utility::EpsilonToZero(nomal));
	}

	return isHit;
}

const bool Cube::IsHit(Cube& _cube)
{
	const Obb& obbB = _cube.GetObb();

	// 各OBBの中心座標（ワールド空間）
	VECTOR centerA = colPos_;
	VECTOR centerB = _cube.GetPos();

	// 2つの中心の差
	VECTOR t = VSub(centerB, centerA);

	// OBBの軸長（半サイズ）
	VECTOR halfA = obb_.halfDiff;
	VECTOR halfB = obbB.halfDiff;

	// 各軸を順にチェック（15軸）
	for (int i = 0; i < static_cast<int>(CUBE_AXIS::MAX); ++i) {
		const VECTOR& axisA = obb_.axis[i];

		// 軸Aの投影量
		float ra = halfA.x * fabs(VDot(axisA, obb_.axis[0])) +
			halfA.y * fabs(VDot(axisA, obb_.axis[1])) +
			halfA.z * fabs(VDot(axisA, obb_.axis[2]));

		float rb = halfB.x * fabs(VDot(axisA, obbB.axis[0])) +
			halfB.y * fabs(VDot(axisA, obbB.axis[1])) +
			halfB.z * fabs(VDot(axisA, obbB.axis[2]));

		if (fabs(VDot(t, axisA)) > ra + rb) return false;
	}

	for (int i = 0; i < static_cast<int>(CUBE_AXIS::MAX); ++i) {
		const VECTOR& axisB = obbB.axis[i];

		float ra = halfA.x * fabs(VDot(axisB, obb_.axis[0])) +
			halfA.y * fabs(VDot(axisB, obb_.axis[1])) +
			halfA.z * fabs(VDot(axisB, obb_.axis[2]));

		float rb = halfB.x * fabs(VDot(axisB, obbB.axis[0])) +
			halfB.y * fabs(VDot(axisB, obbB.axis[1])) +
			halfB.z * fabs(VDot(axisB, obbB.axis[2]));

		if (fabs(VDot(t, axisB)) > ra + rb) return false;
	}

	// 外積軸
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			VECTOR axis = VCross(obb_.axis[i], obbB.axis[j]);

			// 軸が0に近い（平行またはゼロベクトル） → 無視
			if (VSize(axis) < 0.0001f) continue;
			axis = VNorm(axis);

			float ra = halfA.x * fabs(VDot(axis, obb_.axis[0])) +
				halfA.y * fabs(VDot(axis, obb_.axis[1])) +
				halfA.z * fabs(VDot(axis, obb_.axis[2]));

			float rb = halfB.x * fabs(VDot(axis, obbB.axis[0])) +
				halfB.y * fabs(VDot(axis, obbB.axis[1])) +
				halfB.z * fabs(VDot(axis, obbB.axis[2]));

			if (fabs(VDot(t, axis)) > ra + rb) return false;
		}
	}

	// すべての軸で重なっている → 衝突
	return true;
}

const bool Cube::IsHit(Model& _model)
{
	//出来ない
	return false;
}

void Cube::DebugDraw(void)
{
	VECTOR vertices[8];
	CalculateVertices(vertices);

	// 12本のエッジのインデックス
	static const int edges[12][2] = {
		{0,1},{0,2},{0,4}, {1,3},{1,5},
		{2,3},{2,6}, {3,7},
		{4,5},{4,6}, {5,7},{6,7}
	};

	for (int i = 0; i < 12; ++i)
	{
		DrawLine3D(vertices[edges[i][0]], vertices[edges[i][1]], NORMAL_COLOR);
	}

	for (int i = 0; i < 8; ++i) {
		DrawSphere3D(vertices[i], 8, 8, 0xff0000, 0xff0000, false);
	}

	DrawSphere3D(colPos_, 15, 8, 0x00ff55, 0x00ff55, false);
}

void Cube::CalculateVertices(VECTOR outVertices[8])
{
	MATRIX rotMat;
	rotMat = colRot_.ToMatrix();

	//const VECTOR obbMinPos = GetCubeMinWorldPos();
	//const VECTOR obbMaxPos = GetCubeMaxWorldPos();

	const VECTOR obbMinPos = GetCubeMinLocalPos();
	const VECTOR obbMaxPos = GetCubeMaxLocalPos();

	int idx = 0;
	for (int x = 0; x <= 1; ++x)
	{
		for (int y = 0; y <= 1; ++y)
		{
			for (int z = 0; z <= 1; ++z)
			{
				VECTOR local;
				local.x = (x == 0) ? obbMinPos.x : obbMaxPos.x;
				local.y = (y == 0) ? obbMinPos.y : obbMaxPos.y;
				local.z = (z == 0) ? obbMinPos.z : obbMaxPos.z;

				VECTOR world = VTransform(local, rotMat);
				world = VAdd(world, colPos_);

				outVertices[idx++] = world;
				//outVertices[idx++] = local;
			}
		}
	}
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

	const VECTOR obbMinPos = GetCubeMinLocalPos();
	const VECTOR obbMaxPos = GetCubeMaxLocalPos();

	float t = 0.0f;
	float minDiff = FLT_MAX;

	// 線分上の点 P(t) = A + t*(B - A), 0 <= t <= 1
	const int steps = 10;
	for (int i = 0; i <= steps; ++i)
	{
		float ft = static_cast<float>(i) / steps;
		VECTOR endToStart = VSub(_endPos, _startPos);
		VECTOR point = VAdd(_startPos, VScale(endToStart, ft));

		// AABB内の最近接点
		VECTOR clamped = {
			std::max(obbMinPos.x, std::min(point.x, obbMaxPos.x)),
			std::max(obbMinPos.y, std::min(point.y, obbMaxPos.y)),
			std::max(obbMinPos.z, std::min(point.z, obbMaxPos.z))
		};

		float distance = Utility::SqrMagnitudeF(VSub(point, clamped));
		if (distance < minDiff)
		{
			testMemoryVec_ = clamped;
			minDiff = distance;
			t = ft;
		}
	}

	return minDiff;
}

const VECTOR Cube::GetCubeMinWorldPos(void)
{
	return { colPos_.x - obb_.halfDiff.x,colPos_.y - obb_.halfDiff.y,colPos_.z - obb_.halfDiff.z };
}

const VECTOR Cube::GetCubeMaxWorldPos(void)
{
	return { colPos_.x + obb_.halfDiff.x,colPos_.y + obb_.halfDiff.y,colPos_.z + obb_.halfDiff.z };
}

const VECTOR Cube::GetCubeMinLocalPos(void)
{
	return VScale(obb_.halfDiff,-1.0f);
}

const VECTOR Cube::GetCubeMaxLocalPos(void)
{
	return obb_.halfDiff;
}
