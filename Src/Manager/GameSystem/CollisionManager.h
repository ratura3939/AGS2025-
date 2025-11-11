#pragma once
#include<vector>
#include<memory>
#include"AttackManager.h"
#include"../../Object/Common/Collider.h"

class PlayerChara;
class EnemyBase;
class AttackBase;

//10/31メモ
//AttackManagerの更新を完了。このマネージャの改修及び、それに付随したプレイヤーや敵の辺り判定の改修を行う。
//ジャスト回避の判定は設計的にプレイヤー側で行う。

//オブジェクトのコライダーを各自登録するために、シングルトン化
class CollisionManager
{
public:
	static void CreateInstance(void);
	static CollisionManager& GetInstance(void);
	void Destroy(void);

	//要素追加
	void AddCollider(std::weak_ptr<Collider> _col);
	
	//削除予定リスト追加
	void MarkForDelete(const int _colliderIdx);

	//すべてのコライダーの衝突判定
	void UpdateColliders(void);

	/// <summary>
	/// 当たり判定総括
	/// </summary>
	/// <param name="_player">プレイヤー情報</param>
	/// <param name="_enemys">敵情報</param>
	/// <param name="_atks">攻撃情報</param>
	/// <returns>true=スロー演出が入る/false=通常</returns>
	//const bool Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);

private:
	static CollisionManager* instance_;

	CollisionManager(void);
	~CollisionManager(void);

	/// <summary>
	/// プレイヤーの当たり判定関係まとめ
	/// </summary>
	/// <param name="_player">プレイヤー情報</param>
	/// <param name="_atk">攻撃情報</param>
	//void CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks);

	/// <summary>
	/// 敵の当たり判定関係まとめ
	/// </summary>
	/// <param name="_enemy">敵情報</param>
	/// <param name="_atk">攻撃情報</param>
	//void CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemys, std::vector<AttackManager::AttackCollision> _atks);
	
	//要素削除
	void DeleteCollider(void);

	//コライダーの衝突判定
	void CollisionGeometry(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2);

	const bool CheckCollisionTypes(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);

	bool isSlow_;	//スロー演出するか

	std::vector<std::weak_ptr<Collider>> colliders_;

	int colliderCounter_;	//管理番号カウンター
	std::vector<int> deleteColliderIdxs_;	//削除予定コライダーインデックス
};

