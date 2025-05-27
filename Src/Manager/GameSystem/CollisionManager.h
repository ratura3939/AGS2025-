#pragma once
#include<vector>
#include"AttackManager.h"

class PlayerChara;
class EnemyBase;
class AttackBase;

class CollisionManager
{
public:
	CollisionManager(void);
	~CollisionManager(void);

	void Collision(PlayerChara& _player, std::vector<EnemyBase&> _enemys, std::vector<AttackManager::AttackCollision&> _atks);
private:
	/// <summary>
	/// ƒvƒŒƒCƒ„[‚Ì“–‚½‚è”»’èŠÖŒW‚Ü‚Æ‚ß
	/// </summary>
	/// <param name="_player">ƒvƒŒƒCƒ„[î•ñ</param>
	/// <param name="_atk">UŒ‚î•ñ</param>
	void CollisionPlayer(PlayerChara& _player, std::vector<AttackManager::AttackCollision&> _atks);

	/// <summary>
	/// “G‚Ì“–‚½‚è”»’èŠÖŒW‚Ü‚Æ‚ß
	/// </summary>
	/// <param name="_enemy">“Gî•ñ</param>
	/// <param name="_atk">UŒ‚î•ñ</param>
	void CollisionEnemy(std::vector<EnemyBase&>& _enemys, std::vector<AttackManager::AttackCollision&> _atks);
};

