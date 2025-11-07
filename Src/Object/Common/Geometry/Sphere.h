#pragma once
#include "Geometry.h"
class Sphere final :
    public Geometry
{
public:
    Sphere(const VECTOR& _pos, const float _radius);
	~Sphere(void)override;

    const bool IsHit(Geometry& _geo)override;
    const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Cube& _capsule)override;
	const bool IsHit(Model& _model)override;

	const float GetRadius(void)const override { return radius_; }

	void DebugDraw(void)override;

private:
	float radius_;	//”¼Œa
};