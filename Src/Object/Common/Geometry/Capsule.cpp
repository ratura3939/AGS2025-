#include"Sphere.h"
#include"Cube.h"
#include"Model.h"
#include "Capsule.h"

namespace {
	const int NORMAL_COLOR = 0xff0000;
}

Capsule::Capsule(VECTOR& _posTop, VECTOR& _posBottom, const float _radius)
	: Geometry(_posBottom, Quaternion())
	, radius_(_radius)
	, posTop_(_posTop)
	, posBottom_(_posBottom)
{
}

Capsule::~Capsule(void)
{
}

const bool Capsule::IsHit(Geometry& _geo)
{
	return _geo.IsHit(*this);
}

const bool Capsule::IsHit(Sphere& _sphere)
{
	return _sphere.IsHit(*this);
}

const bool Capsule::IsHit(Capsule& _capsule)
{
	VECTOR d1 = VSub(GetPosBottom(), GetPosTop());					// 線分1の方向ベクトル
	VECTOR d2 = VSub(_capsule.GetPosBottom(), _capsule.GetPosTop());	// 線分2の方向ベクトル
	VECTOR r = VSub(GetPosTop(), _capsule.GetPosTop());

	float a = VDot(d1, d1); // d1・d1
	float e = VDot(d2, d2); // d2・d2
	float f = VDot(d2, r);

	float s, t;

	float c = VDot(d1, r);
	float b = VDot(d1, d2);
	float denom = a * e - b * b;

	if (denom != 0.0f)
	{
		s = (b * f - c * e) / denom;
		s = std::clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	t = (b * s + f) / e;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = std::clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = std::clamp((b - c) / a, 0.0f, 1.0f);
	}

	VECTOR c1 = VAdd(GetPosTop(), VScale(d1, s));
	VECTOR c2 = VAdd(_capsule.GetPosTop(), VScale(d2, t));
	float distance = VSize(VSub(c1, c2));

	//衝突したか
	return distance <= (GetRadius() + _capsule.GetRadius());
}

const bool Capsule::IsHit(Cube& _cube)
{
	return _cube.IsHit(*this);
}

const bool Capsule::IsHit(Model& _model)
{
	return _model.IsHit(*this);
}

void Capsule::DebugDraw(void)
{
	DrawFormatString(50, 400, NORMAL_COLOR, "Capsule PosTop X:%f Y:%f Z:%f\nCapsule PosBottom X:%f Y:%f Z:%f", posTop_.x, posTop_.y, posTop_.z, posBottom_.x, posBottom_.y, posBottom_.z);

	// 上の球体
	DrawSphere3D(posTop_, radius_, 5, NORMAL_COLOR, NORMAL_COLOR, false);

	// 下の球体
	DrawSphere3D(posBottom_, radius_, 5, NORMAL_COLOR, NORMAL_COLOR, false);

	//VECTOR dir;
	//VECTOR s;
	//VECTOR e;

	//// 球体を繋ぐ線(X+)
	//dir = GetRight();
	//s = VAdd(posTop_, VScale(dir, radius_));
	//e = VAdd(posBottom_, VScale(dir, radius_));
	//DrawLine3D(s, e, COLOR);

	//// 球体を繋ぐ線(X-)
	//dir = GetLeft();
	//s = VAdd(posTop_, VScale(dir, radius_));
	//e = VAdd(posBottom_, VScale(dir, radius_));
	//DrawLine3D(s, e, NORMAL_COLOR);

	//// 球体を繋ぐ線(Z+)
	//dir = GetForward();
	//s = VAdd(posTop_, VScale(dir, radius_));
	//e = VAdd(posBottom_, VScale(dir, radius_));
	//DrawLine3D(s, e, NORMAL_COLOR);

	//// 球体を繋ぐ線(Z-)
	//dir = GetBack();
	//s = VAdd(posTop_, VScale(dir, radius_));
	//e = VAdd(posBottom_, VScale(dir, radius_));
	//DrawLine3D(s, e, NORMAL_COLOR);

	// カプセルの中心
	DrawSphere3D(VAdd(GetPos(), VScale(posTop_, 2.0f)), 5.0f, 10, NORMAL_COLOR, NORMAL_COLOR, true);
}
