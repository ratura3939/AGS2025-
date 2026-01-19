#pragma once
#include "Geometry.h"

class Capsule :
    public Geometry
{
    public:
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
	float radius_;  //半径
    VECTOR& posTop_;    //カプセル上端位置
	VECTOR& posBottom_; //カプセル下端位置
};

