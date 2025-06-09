#pragma once
#include "SceneBase.h"
#include<memory>

class PlayerManager;
class EnemyManager;
class AttackManager;
class CollisionManager;
class Stage;

class Game :
    public SceneBase
{
public:

	static constexpr int LIMIT_SLOW = 800;
	static constexpr float NOMAL_SPEED_PERCENT = 100.0f;	//通常の割合
	static constexpr float SLOW_SPEED_PERCENT = 25.0f;	//スローの割合(通常時から半分の速度にする)

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
	const int DecideRockEnemy(void);

	void RockOn(void);
	void RockOff(void);
private:

	std::unique_ptr<PlayerManager>player_;			//プレイヤー
	std::unique_ptr<EnemyManager>enemy_;			//敵
	std::unique_ptr<AttackManager>atkMng_;			//攻撃関連
	std::unique_ptr<CollisionManager>collision_;	//判定関連
	std::unique_ptr<Stage>stage_;					//ステージ

	int nearEnemyNum_;
	bool isSlowEffect_;	//スロー演出フラグ
	int slowCnt_;		//スロー演出カウンタ

	void DrawDebug(void);
};

