#include"Model.h"
#include "Sphere.h"


Sphere::Sphere(const VECTOR& _pos, const float _radius)
	: Geometry(_pos, Quaternion())	//回転は関係ないのでデフォルトで初期化
	, radius_(_radius)
{
}

Sphere::~Sphere(void)
{
}

const bool Sphere::IsHit(Sphere& _sphere)
{
	float radiusSum = radius_ + _sphere.GetRadius();
	VECTOR diff = VSub(GetPos(), _sphere.GetPos());

	float distSqr = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	return distSqr <= (radiusSum * radiusSum);
}

const bool Sphere::IsHit(Model& _model)
{
	//モデル側の当たり判定処理に任せる
	return _model.IsHit(*this);
}

void Sphere::DebugDraw(void)
{
	const int divNum = 8;
	const int yellow = 0xffff00;
	DrawSphere3D(GetPos(), radius_, divNum, yellow, yellow, 0);
}
