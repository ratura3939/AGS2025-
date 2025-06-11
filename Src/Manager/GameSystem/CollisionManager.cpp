#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../../Object/Character/CharacterBase.h"
#include"../../Utility/Utility.h"
#include"../Decoration/SoundManager.h"

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
	//ìGÇÃçUåÇÇ…ÇÕëOåÑÅEå„åÑÇ™Ç†ÇÈÇÃÇ≈ÇªÇÍÇÁÇÃîªíËÇ‡çûÇ›Ç≈çsÇ§


	//çUåÇÇÃêîÇæÇØâÒÇ∑
	for (auto& atkCol : _atks) {
		//çUåÇÇ™ÇªÇ‡ÇªÇ‡îªíËçœÇ›ÅEìØëÆÇÃçUåÇÇæÇ¡ÇΩèÍçá
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::PLAYER) {
			//éüÇ÷
			continue;
		}

		//Ç±ÇÍà»ç~ÇÕçUåÇÇÃîªíËÇ™â¬î\Ç»èÛë‘
		//çUåÇà íu
		const VECTOR atkPos = atkCol.attack.lock()->GetPos();
		const float atkRadius = atkCol.info.scale;

		const VECTOR pPos = _player.lock()->GetPos();
		const VECTOR pHeadPos = _player.lock()->GetHeight();

		//çUåÇ(ãÖ)Ç∆ÉLÉÉÉâÉNÉ^Å[(ÉJÉvÉZÉã)ÇÃìñÇΩÇËîªíË
		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, CharacterBase::CHARACTER_RADIUS)) {
			
			//ÉWÉÉÉXÉgâÒî
			if (atkCol.info.IsPreGap()&&_player.lock()->GetState()==PlayerChara::STATE::DODGE) {
   				isSlow_ = true;
				//îªíËçœÇ›Ç…
				atkCol.info.isHit = true;
				continue;
			}

			//ÉWÉÉÉXÉgÉKÅ[Éh


			//î≠ê∂éûä‘íÜÇ…ìñÇΩÇ¡ÇƒÇ¢ÇΩÇÁ
			if (atkCol.info.IsOuccerAttack()) {
				//É_ÉÅÅ[ÉW
				//_player.lock()->Deth();
				SoundManager::GetInstance().Play("Damage");
				//îªíËçœÇ›Ç…
				atkCol.info.HitAttack();
			}
		}
	}
}

void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{

	//çUåÇÇÃêîÇæÇØâÒÇ∑
	for (auto& atkCol : _atks) {
		//çUåÇÇ™ÇªÇ‡ÇªÇ‡îªíËçœÇ›ÅEìØëÆÇÃçUåÇÇæÇ¡ÇΩèÍçá
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::ENEMY) {
			//éüÇ÷
			continue;
		}

		//Ç±ÇÍà»ç~ÇÕçUåÇÇÃîªíËÇ™â¬î\Ç»èÛë‘
		//çUåÇà íu
		const VECTOR atkPos = atkCol.attack.lock()->GetPos();
		const float atkRadius = atkCol.info.scale;

		//ìGÇÃêîÇæÇØâÒÇ∑
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.lock()->GetPos();
			const VECTOR eHeadPos = enemy.lock()->GetHeight();

			//çUåÇ(ãÖ)Ç∆ÉLÉÉÉâÉNÉ^Å[(ÉJÉvÉZÉã)ÇÃìñÇΩÇËîªíË
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, CharacterBase::CHARACTER_RADIUS)) {
				//ìñÇΩÇ¡ÇƒÇ¢ÇΩÇÁ
				enemy.lock()->Deth();
				SoundManager::GetInstance().Play("Damage");
				//îªíËçœÇ›Ç…
				atkCol.info.HitAttack();
			}
		}
	}
}
