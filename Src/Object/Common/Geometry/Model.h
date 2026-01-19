#pragma once
#include "Geometry.h"

//DxLibì‡Ç…Ç†ÇÈìñÇΩÇËîªíËÇäÓÇ…êßçÏ
class Model final :
    public Geometry
{
public:
	Model(const VECTOR& _pos, const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _scl, const int _modelId);
	~Model(void) override;

	const bool IsHit(Geometry& _geo) override;
	const bool IsHit(Line& _line)override;
	const bool IsHit(Sphere& _sphere) override;
	const bool IsHit(Capsule& _capsule) override;
	const bool IsHit(Cube& _cube) override;
	const bool IsHit(Model& _model) override;

	const float GetRadius(void)const override { return 0.0f; }

	void RedyCollision(void) override;

	void DebugDraw(void) override;

private:
	int modelId_;
	const VECTOR& colScale_;
	const Quaternion& colRotLocal_;
};

