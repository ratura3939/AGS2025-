#pragma once
#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Sphere;
class Capsule;
class Model;

class Geometry
{
public:
	virtual ~Geometry(void);

#pragma region 当たり判定各種
	virtual const bool IsHit(Geometry& _geo) = 0;
	virtual const bool IsHit(Sphere& _sphere) = 0;
	virtual const bool IsHit(Capsule& _capsule) = 0;
	virtual const bool IsHit(Model& _model) = 0;
#pragma endregion

	const VECTOR& GetPos(void)const { return colPos_; }
	const Quaternion& GetRot(void)const { return colRot_; }
	const VECTOR& GetNormal(void)const { return normal_; }
	virtual const float GetRadius(void)const = 0;

	void SetNormal(const VECTOR& _normal) { normal_ = _normal; }

	//デバッグ
	virtual void DebugDraw(void) = 0;

protected:
	//コンストラクタは継承先で呼び出す
	Geometry(const VECTOR& _pos,const Quaternion& _rot);

	const VECTOR& colPos_;
	const Quaternion& colRot_;

	VECTOR normal_;
};