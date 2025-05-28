#include"../../Object/Character/Enemy/EnemyBase.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../../Object/Character/CharacterBase.h"
#include"../../Utility/Utility.h"

#include "CollisionManager.h"

using atkM = AttackManager;

CollisionManager::CollisionManager(void)
{
}

CollisionManager::~CollisionManager(void)
{
}

void CollisionManager::Collision(std::weak_ptr<PlayerChara> _player, std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{
	CollisionPlayer(_player, _atks);
	CollisionEnemy(_enemy, _atks);
}

void CollisionManager::CollisionPlayer(std::weak_ptr<PlayerChara> _player, std::vector<AttackManager::AttackCollision> _atks)
{
	//“G‚ÌUŒ‚‚É‚Í‘OŒ„EŒãŒ„‚ª‚ ‚é‚Ì‚Å‚»‚ê‚ç‚Ì”»’è‚àž‚Ý‚Ås‚¤


	//UŒ‚‚Ì”‚¾‚¯‰ñ‚·
	for (auto& atkCol : _atks) {
		//UŒ‚‚ª‚»‚à‚»‚à”»’èÏ‚ÝE“¯‘®‚ÌUŒ‚‚¾‚Á‚½ê‡
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::PLAYER) {
			//ŽŸ‚Ö
			continue;
		}

		//‚±‚êˆÈ~‚ÍUŒ‚‚Ì”»’è‚ª‰Â”\‚Èó‘Ô
		//UŒ‚ˆÊ’u
		const VECTOR atkPos = atkCol.attack.lock()->GetPos();
		const float atkRadius = atkCol.info.scale;

		const VECTOR pPos = _player.lock()->GetPos();
		const VECTOR pHeadPos = _player.lock()->GetHeight();

		//UŒ‚(‹…)‚ÆƒLƒƒƒ‰ƒNƒ^[(ƒJƒvƒZƒ‹)‚Ì“–‚½‚è”»’è
		if (Utility::IsHitSphereCapsule(atkPos, atkRadius, pPos, pHeadPos, CharacterBase::CHARACTER_RADIUS)) {
			//“–‚½‚Á‚Ä‚¢‚½‚ç
			_player.lock()->Deth();
		}
	}
}

void CollisionManager::CollisionEnemy(std::vector<std::weak_ptr<EnemyBase>> _enemy, std::vector<AttackManager::AttackCollision> _atks)
{
	//UŒ‚‚Ì”‚¾‚¯‰ñ‚·
	for (auto& atkCol : _atks) {
		//UŒ‚‚ª‚»‚à‚»‚à”»’èÏ‚ÝE“¯‘®‚ÌUŒ‚‚¾‚Á‚½ê‡
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::ENEMY) {
			//ŽŸ‚Ö
			continue;
		}

		//‚±‚êˆÈ~‚ÍUŒ‚‚Ì”»’è‚ª‰Â”\‚Èó‘Ô
		//UŒ‚ˆÊ’u
		const VECTOR atkPos = atkCol.attack.lock()->GetPos();
		const float atkRadius = atkCol.info.scale;

		//“G‚Ì”‚¾‚¯‰ñ‚·
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.lock()->GetPos();
			const VECTOR eHeadPos = enemy.lock()->GetHeight();

			//UŒ‚(‹…)‚ÆƒLƒƒƒ‰ƒNƒ^[(ƒJƒvƒZƒ‹)‚Ì“–‚½‚è”»’è
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, CharacterBase::CHARACTER_RADIUS)) {
				//“–‚½‚Á‚Ä‚¢‚½‚ç
				enemy.lock()->Deth();
				atkCol.info.isHit = true;
			}
		}
	}
}
