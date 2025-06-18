#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../../Object/Character/CharacterBase.h"
#include"../../Utility/Utility.h"
#include"../Decoration/SoundManager.h"
#include"../Decoration/EffectManager.h"

#include "CollisionManager.h"

using atkM = AttackManager;

CollisionManager::CollisionManager(void)
{
	isSlow_ = false;
}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{
	isSlow_ = false;
	CollisionPlayer(_player, _atks);
	CollisionEnemy(_enemy, _atks);

	return isSlow_;
}

void CollisionManager::CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks)
{
	//敵の攻撃には前隙・後隙があるのでそれらの判定も込みで行う

	const VECTOR pPos = _player.lock()->GetPos();
	const VECTOR pHeadPos = _player.lock()->GetHeight();
	VECTOR efcPos = pHeadPos;
	efcPos.y /= 2.0f;

	//攻撃の数だけ回す
	for (auto& atkCol : _atks) {
		//攻撃がそもそも判定済み・同属の攻撃だった場合
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::PLAYER) {
			//次へ
			continue;
		}

		//これ以降は攻撃の判定が可能な状態
		//攻撃位置
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		

		//攻撃(球)とキャラクター(カプセル)の当たり判定
		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, CharacterBase::CHARACTER_RADIUS)) {
			
			//ジャスト回避
			if (atkCol.info.IsPreGap()&&_player.lock()->GetState()==PlayerChara::STATE::DODGE) {
   				isSlow_ = true;
				//判定済みに
				atkCol.info.isHit = true;
				continue;
			}

			//ジャストガード


			//発生時間中に当たっていたら
			if (atkCol.info.IsOuccerAttack()) {
				//ダメージ
				_player.lock()->Damage(atkCol.attack.pow);
				//ダメージエフェクト・SEの再生
				EffectManager::GetInstance().Play("Damage", efcPos, _player.lock()->GetQua(), 15.0f, 2.5f, "Damage");
				//判定済みに
				atkCol.info.isHit = true;
			}
		}
	}
}

void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{

	//攻撃の数だけ回す
	for (auto& atkCol : _atks) {
		//攻撃がそもそも判定済み・同属の攻撃だった場合
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::ENEMY) {
			//次へ
			continue;
		}

		//これ以降は攻撃の判定が可能な状態
		//攻撃位置
		const VECTOR atkPos = atkCol.attack.pos;
		const float atkRadius = atkCol.info.scale;

		//敵の数だけ回す
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.lock()->GetPos();
			const VECTOR eHeadPos = enemy.lock()->GetHeight();
			//エフェクトの再生位置は胴体ぐらいの想定なので身長の半分くらい
			VECTOR eEfcpos = eHeadPos;
			eEfcpos.y /= 2.0f;

			//攻撃(球)とキャラクター(カプセル)の当たり判定
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, CharacterBase::CHARACTER_RADIUS)) {
				//当たっていたら
				enemy.lock()->Damage(atkCol.attack.pow);
				//ダメージエフェクト・SEの再生
				EffectManager::GetInstance().Play("Damage", eEfcpos, enemy.lock()->GetQua(), 15.0f, 2.5f, "Damage");
				//武器エフェクトの再生(現在は斬撃だけなので確定でこれを流すようになってる)
				EffectManager::GetInstance().Play("Sword", eEfcpos, enemy.lock()->GetQua(), 50.0f,1.5f);
				//判定済みに
				atkCol.info.isHit = true;
			}
		}
	}
}
