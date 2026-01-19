#pragma once
#include "Geometry.h"
class Line :
    public Geometry
{
public:
	Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _startPos, const VECTOR& _endPos);
	~Line(void) override;

	const bool IsHit(Geometry& _geo) override;
	const bool IsHit(Line& _line)override;
	const bool IsHit(Sphere& _sphere) override;
	const bool IsHit(Capsule& _capsule) override;
	const bool IsHit(Cube& _cube) override;
	const bool IsHit(Model& _model) override;

	const float GetRadius(void)const override { return 0.0f; }

	const VECTOR& GetStartPos(void)const { return startPos_; }
	const VECTOR& GetEndPos(void)const { return endPos_; }
	const VECTOR& GetLine(void)const { return VSub(endPos_, startPos_); }
	const float GetLength(void)const { return length_; }

	void DebugDraw(void) override;

private:
	VECTOR startPos_;	//始点
	VECTOR endPos_;		//終点
	float length_;		//長さ
};

