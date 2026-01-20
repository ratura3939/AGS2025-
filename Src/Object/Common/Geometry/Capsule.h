#pragma once
#include "Geometry.h"

class Capsule :
    public Geometry
{
public:
	//線分とカプセル球体部分の当たり判定結果
    struct CollisionInfoLineToCapsuleParts {
		bool isHit;            //当たったかどうか
		float closestRateLine; //線分上の最近接点の割合(0～1)
    };

    Capsule(VECTOR& _posTop, VECTOR& _posBottom, const float _radius);
    ~Capsule(void) override;

    const bool IsHit(Geometry& _geo)override;
    const bool IsHit(Line& _line)override;
    const bool IsHit(Sphere& _sphere)override;
    const bool IsHit(Capsule& _capsule)override;
    const bool IsHit(Cube& _cube)override;
    const bool IsHit(Model& _model)override;

    const VECTOR& GetPosTop(void)const { return posTop_; }
    const VECTOR& GetPosBottom(void)const { return posBottom_; }
    const float GetRadius(void)const override { return radius_; }

	void DebugDraw(void) override;

private:
    const CollisionInfoLineToCapsuleParts CheckLineToSphere(const VECTOR& _lineStart, const VECTOR& _lineEnd, const VECTOR& _lineVec, const VECTOR& _center);
    const CollisionInfoLineToCapsuleParts CheckLineToCylinder(const VECTOR& _lineStart, const VECTOR& _lineEnd, const VECTOR& _lineVec);
    void ReflectHitInfoFromCapsuleParts(const CollisionInfoLineToCapsuleParts& _capsulePartInfo, float& _min);

	float radius_;  //半径
    VECTOR& posTop_;    //カプセル上端位置
	VECTOR& posBottom_; //カプセル下端位置
};

