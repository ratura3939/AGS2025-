#pragma once
#include<DxLib.h>
#include<set>
#include<memory>
#include"../Common/Geometry/Geometry.h"

class ActorBase;

class Collider
{
public:
	enum class MASTER_TYPE
	{
		PLAYER
		,ENEMY
		,STAGE
		,OBJECT
	};

	Collider(ActorBase& _master, const std::set<MASTER_TYPE> _types, std::unique_ptr<Geometry> _geo, const std::set<MASTER_TYPE> _noHitTypes = {});
	~Collider(void);

	//衝突処理
	void OnHit(Collider& _col);

	//タイプの取得
	const std::set<MASTER_TYPE>& GetTypes(void) const { return types_; }
	const std::set<MASTER_TYPE>& GetNoHitTypes(void) const { return noHitTypes_; }

	//形状の取得
	Geometry& GetGeometry(void) const { return *geometry_; }

protected:
	//持ち主
	ActorBase& master_;

	//形状
	std::unique_ptr<Geometry> geometry_;

	//自身のタイプ
	std::set<MASTER_TYPE> types_;

	//衝突判定を行わない種類
	std::set<MASTER_TYPE> noHitTypes_;
};

