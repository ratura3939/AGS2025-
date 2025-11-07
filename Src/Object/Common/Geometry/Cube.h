#pragma once
#include "Geometry.h"
class Cube :
    public Geometry
{
public:
	enum class CUBE_AXIS {
		RIGHT
		,UP
		,FORWARD
		,MAX
	};

	//バウンディングボックス
	struct Obb{
		VECTOR centerPos;
		VECTOR halfDiff;
		VECTOR axis[static_cast<int>(CUBE_AXIS::MAX)];	//回転情報
	};

    Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _min, const VECTOR& _max);
    Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _squareSize);

	~Cube(void)override;

	const bool IsHit(Geometry& _geo)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Cube& _capsule)override;
	const bool IsHit(Model& _model)override;

	const float GetRadius(void)const override { return 0.0f; }

	void DebugDraw(void)override;

	const Obb& GetObb(void)const { return obb_; }

private:
	Obb obb_;	//バウンディングボックス

	// クォータニオンから回転軸を計算
	void UpdateObbAxis(void);

	const VECTOR& GetAxis(const CUBE_AXIS& _order)const;

	const float ClosestPointDiff(const VECTOR& _startPos, const VECTOR& _endPos);

	const VECTOR GetCubeMinPos(void);
	const VECTOR GetCubeMaxPos(void);
};

