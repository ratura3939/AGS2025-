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

void CollisionManager::Collision(PlayerChara& _player, std::vector<EnemyBase&> _enemy, std::vector<AttackManager::AttackCollision&> _atks)
{
	//CollisionPlayer(_player, _atks);
	CollisionEnemy(_enemy, _atks);
}

void CollisionManager::CollisionPlayer(PlayerChara& _player, std::vector<AttackManager::AttackCollision&> _atks)
{
	//UŒ‚‚Ì”‚¾‚¯‰ñ‚·
	for (auto& atkCol : _atks) {
		//UŒ‚‚ª‚»‚à‚»‚à”»’èÏ‚İE“¯‘®‚ÌUŒ‚‚¾‚Á‚½ê‡
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::PLAYER) {
			//Ÿ‚Ö
			continue;
		}
	}
}

void CollisionManager::CollisionEnemy(std::vector<EnemyBase&>& _enemy, std::vector<AttackManager::AttackCollision&> _atks)
{
	//UŒ‚‚Ì”‚¾‚¯‰ñ‚·
	for (auto& atkCol : _atks) {
		//UŒ‚‚ª‚»‚à‚»‚à”»’èÏ‚İE“¯‘®‚ÌUŒ‚‚¾‚Á‚½ê‡
		if (atkCol.info.isHit == true || atkCol.info.master == atkM::ATTACK_MASTER::ENEMY) {
			//Ÿ‚Ö
			continue;
		}

		//‚±‚êˆÈ~‚ÍUŒ‚‚Ì”»’è‚ª‰Â”\‚Èó‘Ô
		//UŒ‚ˆÊ’u
		const VECTOR atkPos = atkCol.attack.GetPos();
		const float atkRadius = atkCol.info.scale;

		//“G‚Ì”‚¾‚¯‰ñ‚·
		for (auto& enemy : _enemy) {
			const VECTOR ePos = enemy.GetPos();
			const VECTOR eHeadPos = enemy.GetHeight();

			//UŒ‚(‹…)‚ÆƒLƒƒƒ‰ƒNƒ^[(ƒJƒvƒZƒ‹)‚Ì“–‚½‚è”»’è
			if (Utility::IsHitSphereCapsule(atkPos, atkRadius, ePos, eHeadPos, CharacterBase::CHARACTER_RADIUS)) {
				//“–‚½‚Á‚Ä‚¢‚½‚ç
				enemy.Deth();
			}
		}
	}
}
