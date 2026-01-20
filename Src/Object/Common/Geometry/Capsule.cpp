#include"Line.h"
#include"Sphere.h"
#include"Cube.h"
#include"Model.h"
#include "Capsule.h"

namespace {
	const int NORMAL_COLOR = 0xff0000;
	const float INVALID_VALUE = 1e9f;
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

const bool Capsule::IsHit(Line& _line)
{
	bool isHit = false;

	//最も近い点の割合(0～1)
	float min = INVALID_VALUE;

	//接触点を求めないといけないため、細かく調べる(上端球、下端球、円柱)
	//線分情報
	const VECTOR start = _line.GetStartPos();
	const VECTOR end = _line.GetEndPos();
	const VECTOR lineVec = _line.GetLineVec();

	//各球体との当たり判定
	CollisionInfoLineToCapsuleParts topSphere=CheckLineToSphere(start, end, lineVec, posTop_);
	CollisionInfoLineToCapsuleParts bottomSphere=CheckLineToSphere(start, end, lineVec, posBottom_);
	//円柱との当たり判定
	CollisionInfoLineToCapsuleParts cylinder = CheckLineToCylinder(start, end, lineVec);

	//それぞれの結果反映
	ReflectHitInfoFromCapsuleParts(topSphere, min);
	ReflectHitInfoFromCapsuleParts(bottomSphere, min);
	ReflectHitInfoFromCapsuleParts(cylinder, min);

	if(min>=0.0f && min <=1.0f){
		//接触点、法線設定
		const VECTOR hitPoint = VAdd(start, VScale(lineVec, min));
		_line.SetHitPoint(hitPoint);

		//カプセル軸ベクトル
		const VECTOR capVec = VNorm(VSub(posBottom_, posTop_));
		//接触点からカプセル軸への射影ベクトルを求める
		const VECTOR startToHitPoint = VSub(hitPoint, posTop_);
		const float projectionLength = VDot(startToHitPoint, capVec);
		VECTOR projectionVec = VScale(capVec, projectionLength);
		//接触点からカプセル軸への最短ベクトルを求める
		VECTOR closestVec = VSub(startToHitPoint, projectionVec);
		_line.SetHitNormal(VNorm(closestVec));

		//衝突した
		isHit = true;
	}

	return isHit;
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

	if (denom != 0.0f){
		s = (b * f - c * e) / denom;
		s = std::clamp(s, 0.0f, 1.0f);
	}
	else{
		s = 0.0f;
	}

	t = (b * s + f) / e;
	if (t < 0.0f){
		t = 0.0f;
		s = std::clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f){
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

const Capsule::CollisionInfoLineToCapsuleParts Capsule::CheckLineToSphere(const VECTOR& _lineStart, const VECTOR& _lineEnd, const VECTOR& _lineVec, const VECTOR& _center)
{
	CollisionInfoLineToCapsuleParts retInfo;
	retInfo.isHit = false;
	retInfo.closestRateLine = INVALID_VALUE;	//範囲外で初期化

	const VECTOR startToSphere = VSub(_center, _lineStart);

	//解の公式を活用して判定
	const float a = VDot(_lineVec, _lineVec);
	const float b = -2.0f * VDot(startToSphere, _lineVec);
	const float c = VDot(startToSphere, startToSphere) - (radius_ * radius_);
	//判別式
	//discriminant==0 : 1解(接触)
	//discriminant>0  : 2解(交差)
	//discriminant<0  : 解なし(非接触)
	const float discriminant = (b * b) - (4.0f * a * c);

	//解が存在するか
	if (discriminant >= 0) {
		const float sqrtDiscriminant = sqrtf(discriminant);

		//2解(衝突点が線分の何割目にあるか)を求める(t1=手前、t2=奥)
		const float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
		const float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

		//線分上に解が存在するか(今回のゲームの仕様上手前の点のみ採用)
		if ((t1 >= 0.0f && t1 <= 1.0f)) {
			//設定
			retInfo.closestRateLine = t1;
			retInfo.isHit = true;
		}
	}

	return retInfo;
}

const Capsule::CollisionInfoLineToCapsuleParts Capsule::CheckLineToCylinder(const VECTOR& _lineStart, const VECTOR& _lineEnd, const VECTOR& _lineVec)
{
	CollisionInfoLineToCapsuleParts retInfo;
	retInfo.isHit = false;
	retInfo.closestRateLine = INVALID_VALUE;	//範囲外で初期化

	const VECTOR capVec = VNorm(VSub(posBottom_, posTop_));	//カプセルの軸方向ベクトル
	const VECTOR startToCapBtm = VSub(_lineStart, posBottom_);

	//カプセル軸に沿った成分を取り除く
	const VECTOR lineAtCapVec = VScale(capVec, VDot(_lineVec, capVec));
	const VECTOR startToCapBtmAtCapVec = VScale(capVec, VDot(startToCapBtm, capVec));

	//解の公式を活用して判定
	float a = VDot(lineAtCapVec, lineAtCapVec);
	float b = 2.0f * VDot(lineAtCapVec, startToCapBtmAtCapVec);
	float c = VDot(startToCapBtmAtCapVec, startToCapBtmAtCapVec) - (radius_ * radius_);

	//判別式
	//discriminant==0 : 1解(接触)
	//discriminant>0  : 2解(交差)
	//discriminant<0  : 解なし(非接触)
	float discriminant = (b * b) - (4.0f * a * c);

	//絵画存在するとき
	if (discriminant >= 0) {
		const float sqrtDiscriminant = sqrtf(discriminant);

		const float t1 = (-b - sqrtDiscriminant) / (2.0f * a);

		//範囲内に解が存在するか
		if ((t1 >= 0.0f && t1 <= 1.0f)) {
			retInfo.isHit = true;
			retInfo.closestRateLine = t1;
		}
	}
	return retInfo;
}

void Capsule::ReflectHitInfoFromCapsuleParts(const CollisionInfoLineToCapsuleParts& _capsulePartInfo, float& _min)
{
	//当たっていたら
	if (_capsulePartInfo.isHit) {
		//より近い点なら反映
		if (_capsulePartInfo.closestRateLine < _min) {
			_min = _capsulePartInfo.closestRateLine;
		}
	}
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
