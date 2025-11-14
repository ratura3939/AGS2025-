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

void CollisionManager::UpdateColliders(void)
{
	int idx = 0;
	
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

//const bool CollisionManager::Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
//{
//	isSlow_ = false;
//	CollisionPlayer(_player, _atks);
//	CollisionEnemy(_enemy, _atks);
//
//	return isSlow_;
//}
//
//void CollisionManager::CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks)
//{
//	//敵の攻撃には前隙・後隙があるのでそれらの判定も込みで行う
//
//	const VECTOR pPos = _player.lock()->GetPos();		//プレイヤー位置(足元)
//	const VECTOR pHeadPos = _player.lock()->GetHeight();//プレイヤー位置(頭上)
//	VECTOR efcPos = pHeadPos;							//エフェクト位置(胴体)
//	efcPos.y /= 2.0f;
//
//	const std::string name = _player.lock()->GetSpeciesName();	//個体名
//
//	//攻撃の数だけ回す
//	for (auto& atkCol : _atks) {
//		//攻撃が判定済み・同陣営の攻撃だった場合
//		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::PLAYER) {
//			//次へ
//			continue;
//		}
//
//		//これ以降は攻撃の判定が可能な状態
//		//攻撃位置・大きさ
//		const VECTOR atkPos = atkCol.attack.pos;
//		const float atkRadius = atkCol.info.scale;
//
//		const float DmgEfcScl = 25.0f;
//		const float DmgEfcSpeed = 2.5;
//		
//
//		//攻撃(球)とキャラクター(カプセル)の当たり判定
//		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, _player.lock()->GetCollisionRadius())) {
//			
//			//回避可能時間に当たっていたら
//			if (atkCol.info.IsPreGap()) {
//
//				//ジャスト回避できるよ！！という予測を出すようにする。
//				if (!atkCol.info.isAllert) {
//					_player.lock()->SetAtkAllert();
//					SoundManager::GetInstance().Play("Allert");
//					atkCol.info.isAllert = true;
//				}
//
//				//回避していたら
//				if (_player.lock()->GetState() == PlayerChara::STATE::DODGE) {
//					//スローに
//					isSlow_ = true;
//					//ジャスト回避の効果音流す
//					SoundManager::GetInstance().Play("JustDodge");
//					//判定済みに
//					atkCol.info.isHit = true;
//				}
//				continue;
//			}
//
//			//ジャストガード
//
//
//			//発生時間中に当たっていたら
//			if (atkCol.info.IsOccerAttack()) {
//				//ダメージ
//				_player.lock()->Damage(atkCol.attack.pow);
//				//ダメージエフェクト・SEの再生
//				EffectManager::GetInstance().Play(name,"Damage", efcPos, _player.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
//				//判定済みに
//				atkCol.info.isHit = true;
//			}
//		}
//	}
//	//テキトーな移動制限
//	if (Utility::MagnitudeF(pPos) > 5000.0f) {
//		_player.lock()->SetPrePos();
//	}
//}
//
//void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
//{
//
//	//攻撃の数だけ回す
//	for (auto& atkCol : _atks) {
//		//攻撃がそもそも判定済み・同属の攻撃だった場合
//		if (atkCol.info.isHit == true || atkCol.info.group == atkM::ATTACK_MASTER::ENEMY) {
//			//次へ
//			continue;
//		}
//
//		//これ以降は攻撃の判定が可能な状態
//		//攻撃位置
//		const VECTOR atkPos = atkCol.attack.pos;
//		const float atkRadius = atkCol.info.scale;
//
//		const float DmgEfcScl = 25.0f;
//		const float DmgEfcSpeed = 2.5;
//		const float SwordEfcScl = 50.0f;
//		const float SwordEfcSpeed = 1.5;
//
//		//敵の数だけ回す
//		for (auto& enemy : _enemy) {
//			const VECTOR ePos = enemy.lock()->GetPos();
//			const VECTOR eHeadPos = enemy.lock()->GetHeight();
//			//エフェクトの再生位置は胴体ぐらいの想定なので身長の半分くらい
//			VECTOR eEfcpos = eHeadPos;
//			eEfcpos.y /= 2.0f;
//
//			const std::string name = enemy.lock()->GetSpeciesName();
//
//			//攻撃(球)とキャラクター(カプセル)の当たり判定
//			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, enemy.lock()->GetCollisionRadius())) {
//				//当たっていたら
//				enemy.lock()->Damage(atkCol.attack.pow);
//				//ダメージエフェクト・SEの再生
//				EffectManager::GetInstance().Play(name,"Damage", eEfcpos, enemy.lock()->GetQua(), DmgEfcScl, DmgEfcSpeed, "Damage");
//				//武器エフェクトの再生(現在は斬撃だけなので確定でこれを流すようになってる)
//				EffectManager::GetInstance().Play(name, "Sword", eEfcpos, enemy.lock()->GetQua(), SwordEfcScl, SwordEfcSpeed);
//				//判定済みに
//				atkCol.info.isHit = true;
//			}
//			//テキトーな移動制限
//			if (Utility::MagnitudeF(ePos) > 5000.0f) {
//				enemy.lock()->SetPrePos();
//			}
//		}
//	}
//}

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
