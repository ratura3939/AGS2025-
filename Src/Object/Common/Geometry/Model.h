#pragma once
#include "Geometry.h"
class Model final :
    public Geometry
{
public:
	Model(const VECTOR& _pos, const Quaternion& _rot,const int _modelId);
	~Model(void) override;

	const bool IsHit(Geometry& _geo) override;
	const bool IsHit(Sphere& _sphere) override;
	const bool IsHit(Model& _model) override;

	void DebugDraw(void) override;

private:
	int modelId_;
	MV1_COLL_RESULT_POLY_DIM hitSphereInfo_;
};

