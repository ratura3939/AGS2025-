#pragma once
#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Sphere;
class Capsule;
class Cube;
class Model;

class Geometry
{
public:
	virtual ~Geometry(void);

#pragma region 当たり判定各種
	virtual const bool IsHit(Geometry& _geo) = 0;
	virtual const bool IsHit(Sphere& _sphere) = 0;
	virtual const bool IsHit(Capsule& _capsule) = 0;
	virtual const bool IsHit(Cube& _capsule) = 0;
	virtual const bool IsHit(Model& _model) = 0;
#pragma endregion

	const VECTOR& GetPos(void)const { return colPos_; }
	const Quaternion& GetRot(void)const { return colRot_; }
	const VECTOR& GetHitNormal(void)const { return hitNormal_; }
	const VECTOR& GetHitPoint(void)const { return hitPoint_; }
	virtual const float GetRadius(void)const = 0;

	void SetHitNormal(const VECTOR& _normal) { hitNormal_ = _normal; }
	void SetHitPoint(const VECTOR& _point) { hitPoint_ = _point; }

	//判定前の準備
	virtual void RedyCollision(void);

	//デバッグ
	virtual void DebugDraw(void) = 0;

protected:
	//コンストラクタは継承先で呼び出す
	Geometry(const VECTOR& _pos,const Quaternion& _rot);

	const VECTOR& colPos_;
	const Quaternion& colRot_;

	VECTOR hitNormal_;
	VECTOR hitPoint_;
};