#pragma once
#include<vector>
#include<memory>
#include"AttackManager.h"
#include"../../Object/Common/Collider.h"


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
	void MarkForDelete(std::weak_ptr<Collider> _col);

	void DeleteAllCollider(void);

	//すべてのコライダーの衝突判定
	void UpdateColliders(void);

	//攻撃マネージャとの連携
	void SetAttackManager(const std::weak_ptr<AttackManager>& _atkMng) { atkMng_ = _atkMng; }
	void UseAllert(const std::string& _atkName);
	void UseAttack(const std::string& _atkName);

	void Reset(void);

private:
	static CollisionManager* instance_;

	std::weak_ptr<AttackManager> atkMng_;

	CollisionManager(void);
	~CollisionManager(void);
	
	//要素削除
	void DeleteCollider(void);

	//コライダーの衝突判定
	void CollisionGeometry(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2);

	const bool CheckCollisionTypes(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);

	bool isSlow_;	//スロー演出するか

	std::vector<std::weak_ptr<Collider>> colliders_;
	std::vector<std::weak_ptr<Collider>> deleteList_;	//削除予定コライダーインデックス
};

