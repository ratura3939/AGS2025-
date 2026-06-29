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

	enum class CUBE_DIR {
		MAX
		,X
		,Y
		,Z
	};

	//バウンディングボックス
	struct Obb{
		VECTOR halfDiff;		//大きさ
		VECTOR axis[static_cast<int>(CUBE_AXIS::MAX)];	//回転情報
	};

	static constexpr int CUBE_EDGE_NUM = 12;	//辺の数
	static constexpr int CUBE_VERTEX_NUM = 8;	//頂点の数

	// 12本のエッジのインデックス
	static constexpr int EDGES_POINT[12][2] = {
		{0,1},{0,2},{0,4}, {1,3},{1,5},
		{2,3},{2,6}, {3,7},
		{4,5},{4,6}, {5,7},{6,7}
	};

    Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _halfSize);
	~Cube(void)override;

	const bool IsHit(Geometry& _geo)override;
	const bool IsHit(Line& _line)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Model& _model)override;

	const float GetRadius(void)const override { return 0.0f; }

	void DebugDraw(void)override;
	void CalculateVertices(VECTOR outVertices[8]);

	//バウンディングボックス取得
	const Obb& GetObb(void)const { return obb_; }

	//サイズ設定
	void SetObbHalfDiff(const VECTOR& _halfDiff) { obb_.halfDiff = _halfDiff; }

	const VECTOR& GetObbHalfDiff(void)const { return obb_.halfDiff; }

private:
	// クォータニオンから回転軸を計算
	void UpdateObbAxis(void);

	const VECTOR& GetAxis(const CUBE_AXIS& _order)const;

	// スラブ法で最近接点の距離を計算
	const float ClosestPointDiff(const VECTOR& _startPos, const VECTOR& _endPos);

	//スラブ法の補助関数
	bool IntersectSlab(const VECTOR& _start, const VECTOR& _dir, float& _tmin, float& _tmax, CUBE_DIR& _hitDir, float& _hitSide);
	void SetUseVectorInfo(const CUBE_DIR& _useCubeDir, const VECTOR& _startVec, const VECTOR& _dirVec, const VECTOR& _localCubeMin, const VECTOR& _localCubeMax, float& _useStart, float& _useDir, float& _useMin, float& _useMax);

	//ワールド座標をCubeのローカル座標に変換
	const VECTOR WorldToLocal(const VECTOR& _worldPos);

	//ワールド座標系での最小・最大座標取得
	const VECTOR GetCubeMinWorldPos(void);
	const VECTOR GetCubeMaxWorldPos(void);
	//ローカル座標系での最小・最大座標取得
	const VECTOR GetCubeMinLocalPos(void);
	const VECTOR GetCubeMaxLocalPos(void);

	//傾きを考慮した法線方向の取得
	const VECTOR GetTiltAdjustedNormal(const VECTOR& _worldNormal);

	Obb obb_;	//バウンディングボックス
	VECTOR localHitPos_;	//ローカル当たり位置
	VECTOR loaclDepthVec_;	//ローカルめり込み量
};