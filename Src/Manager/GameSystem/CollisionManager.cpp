#include"../../Object/Common/Collider.h"
#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../../Object/Character/CharacterBase.h"
#include"../../Utility/Utility.h"
#include"../Decoration/SoundManager.h"
#include"../Generic/ResourceManager.h"
#include"../Decoration/EffectManager.h"

#include "CollisionManager.h"

using atkM = AttackManager;

CollisionManager* CollisionManager::instance_ = nullptr;

void CollisionManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new CollisionManager();
	}
}

CollisionManager& CollisionManager::GetInstance(void)
{
	return *instance_;
}

void CollisionManager::Destroy(void)
{
	if (instance_ != nullptr) {
		delete instance_;
		instance_ = nullptr;
	}
}

CollisionManager::CollisionManager(void)
	: colliderCounter_(0)
	, isSlow_(false)
{
}

CollisionManager::~CollisionManager(void)
{
}

void CollisionManager::AddCollider(std::weak_ptr<Collider> _col)
{
	//管理番号付与→追加→カウンタ増加
	_col.lock()->SetManagementNumber(colliderCounter_);
	colliders_.push_back(_col);
	colliderCounter_++;
}

void CollisionManager::DeleteCollider(void)
{
	//削除予定リストに入っているものを削除
	for (int deleteCounter = 0; deleteCounter < static_cast<int>(deleteColliderIdxs_.size()); deleteCounter++) {
		const int deleteIdx = deleteColliderIdxs_[deleteCounter];

		//削除以降の管理番号を一つ手前に
		for (int idx = deleteIdx + 1; idx < static_cast<int>(colliders_.size()); idx++) {
			colliders_[idx].lock()->DecreaseManagementNuber();
		}
		//削除
		colliders_.erase(colliders_.begin() + deleteIdx);
		colliderCounter_--;

		for (int i = deleteCounter + 1; i < static_cast<int>(deleteColliderIdxs_.size()); i++) {
			//削除した分インデックスをずらす
			if (deleteColliderIdxs_[i] > deleteIdx) {
				deleteColliderIdxs_[i]--;
			}
		}
	}
}

void CollisionManager::MarkForDelete(const int _colliderIdx)
{
	deleteColliderIdxs_.push_back(_colliderIdx);
}

void CollisionManager::DeleteAllCollider(void)
{
	colliders_.clear();
}

void CollisionManager::UpdateColliders(void)
{
	int idx = 0;
	
	for (auto& col : colliders_) {
		col.lock()->GetGeometry().RedyCollision();
	}

	for (auto& col : colliders_) {
		//使わないものは判定しない
		if (!col.lock()->IsUseThis()) {
			idx++;
			continue;
		}

		//現在以降の物としか判定をとらない(既に判定を行っているため)
		for(int i = idx + 1; i < static_cast<int>(colliders_.size()); i++) {
			//使わないものは判定しない
			if (!colliders_[i].lock()->IsUseThis())continue;
			CollisionGeometry(col, colliders_[i]);
		}
		idx++;
	}

	//削除予定リストに入っているものを削除
	DeleteCollider();
	//削除予定リストクリア
	deleteColliderIdxs_.clear();
}

void CollisionManager::UseAllert(const std::string& _atkName)
{
	atkMng_.lock()->UseAllertCollision(_atkName);
}

void CollisionManager::UseAttack(const std::string& _atkName)
{
	atkMng_.lock()->UseAttackCollision(_atkName);
}

void CollisionManager::CollisionGeometry(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2)
{
	//タイプの確認(双方)
	if (!CheckCollisionTypes(_col1, _col2))return;
	if (!CheckCollisionTypes(_col2, _col1))return;
	
	//形状同士の当たり判定
	auto& geo1 = _col1.lock()->GetGeometry();
	auto& geo2 = _col2.lock()->GetGeometry();

	//衝突があった場合
	if (geo1.IsHit(geo2)) {
		_col1.lock()->OnHit(_col2);
		_col2.lock()->OnHit(_col1);
	}
}

const bool CollisionManager::CheckCollisionTypes(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)
{
	for (auto& type : _col1.lock()->GetTags()) {
		for (auto& type2 : _col2.lock()->GetNoHitTags()) {
			//相手の無効タイプに自分のタイプがあったら処理しない
			if (type == type2) {
				return false;
			}
		}
	}
	return true;
}
