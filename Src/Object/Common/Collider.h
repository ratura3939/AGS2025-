#pragma once
#include<DxLib.h>
#include<set>
#include<memory>
#include"../Common/Geometry/Geometry.h"

class ActorBase;

class Collider
{
public:
	enum class COL_TAG
	{
		PLAYER		//プレイヤー
		,ENEMY		//敵
		,STAGE		//ステージ
		,OBJECT		//オブジェクト
		,ATACK		//攻撃
		,PREATTACK	//攻撃前隙
	};

	Collider(ActorBase& _master, const std::set<COL_TAG> _tags, std::unique_ptr<Geometry> _geo, const std::set<COL_TAG> _noHitTags = {});
	~Collider(void);

	//衝突処理
	void OnHit(std::weak_ptr<Collider> _col);

	//タイプの取得
	const std::set<COL_TAG>& GetTags(void) const { return tags_; }
	const std::set<COL_TAG>& GetNoHitTags(void) const { return noHitTags_; }

	//タグの追加
	void AddTag(const COL_TAG& _tag) { tags_.insert(_tag); }
	void AddNoHitTag(const COL_TAG& _tag) { noHitTags_.insert(_tag); }

	//タグの削除
	void DeleteTag(const COL_TAG& _tag);
	void DeleteNoHitTag(const COL_TAG& _tag);

	//形状の取得
	Geometry& GetGeometry(void) const { return *geometry_; }

	//衝突判定に使用するか
	const bool IsUseThis(void) const { return isCollision_; }

	//衝突判定の使用設定
	void SetUseThis(const bool _isUse) { isCollision_ = _isUse; }

	//デバッグ
	void DrawDebugCollider(void);

protected:
	//持ち主
	ActorBase& master_;

	//形状
	std::unique_ptr<Geometry> geometry_;

	//自身のタグ
	std::set<COL_TAG> tags_;

	//衝突判定を行わない種類
	std::set<COL_TAG> noHitTags_;

	//衝突判定を行うか
	bool isCollision_;
};

