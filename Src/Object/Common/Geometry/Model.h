#pragma once
#include "Geometry.h"

//DxLib“à‚É‚ ‚é“–‚½‚è”»’è‚ğŠî‚É§ì
class Model final :
    public Geometry
{
public:
	Model(const VECTOR& _pos, const Quaternion& _rot,const int _modelId);
	~Model(void) override;

	const bool IsHit(Geometry& _geo) override;
	const bool IsHit(Sphere& _sphere) override;
	const bool IsHit(Capsule& _capsule) override;
	const bool IsHit(Cube& _cube) override;
	const bool IsHit(Model& _model) override;

	const float GetRadius(void)const override { return 0.0f; }

	void DebugDraw(void) override;

private:
	int modelId_;
};

