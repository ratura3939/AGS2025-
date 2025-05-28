#pragma once
#include "SceneBase.h"
#include<memory>

class PlayerManager;
class EnemyManager;
class AttackManager;
class CollisionManager;

class Game :
    public SceneBase
{
public:

	Game(void);
	~Game(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	/// <summary>
	/// 攻撃の基礎情報登録(ゆくゆくは外部データにしたい)
	/// </summary>
	void AttackDataInit(void);

	/// <summary>
	/// ロックオンの対象決め
	/// </summary>
	/// <param name=""></param>
	void DecideRockEnemy(void);
private:

	std::unique_ptr<PlayerManager>player_;			//プレイヤー
	std::unique_ptr<EnemyManager>enemy_;			//敵
	std::unique_ptr<AttackManager>atkMng_;			//攻撃関連
	std::unique_ptr<CollisionManager>collision_;	//判定関連
	int nearEnemyNum_;

	void DrawDebug(void);
};

