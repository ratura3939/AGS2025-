#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"
#include"../../Manager/Generic/SceneManager.h"

class Game;
class StageManager;
class EnemyManager;
class PlayerChara;
class AttackManager;
class LockOnManager;
class AbilityManager;

class PlayerManager
{
public:
	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;
	//デバッグ用
	static constexpr int ATTACK_TIME = 50;

	//状態上限時間
	static constexpr int LIMIT_AVOID_STATE = 30;	//回避

	PlayerManager(Game& _gameScene, EnemyManager& _enemy, AttackManager& _atk, StageManager& _stage);;
	~PlayerManager(void);

	void Init(void);	//初期化
	void Update(void);	//更新
	void Draw(void);	//描画
	void Release(void);	//解放

	//キャラクター取得
	std::weak_ptr<PlayerChara> GetPlayer(void);

	//位置・回転取得
	const VECTOR& GetPos(void)const;	//座標
	const Quaternion& GetQua(void);		//回転
	const VECTOR& GetFocusPoint(void);	//注視点
	const VECTOR& GetFollowPosForUseMagnet(void);	//マグネット使用時の注視点

	const bool IsAlive(void)const;		//生存しているか
	const bool IsUseAbility(void)const;	//アビリティを使用中か
	const bool IsUseMagnet(void)const;	//マグネットを使用しているか

	//ロックオン・オフ時に必要な処理
	void RedyLockOn(void);	//ロックオン開始処理
	void RedyLockOff(void);	//ロックオフ開始処理

	//操作UIの設定
	void SetOperationUI(const SceneManager::CNTL& _operation);

private:
	/// <summary>
	/// プレイヤーからの入力受付関係
	/// </summary>
	void UserInput(void);

	/// <summary>
	/// 回避可能方向に移動入力があるか
	/// </summary>
	const bool IsDudgeMove(void)const;

	//回避処理
	void DoDudge(void);

	//カウンタの設定
	void RedyStateCount(const int _limit);

	Game& scene_;	//ゲームクラス参照
	AttackManager& atkMng_;

	std::shared_ptr<PlayerChara> character_;//キャラクター
	std::unique_ptr<LockOnManager>lockOn_;	//ロックオン関係
	std::unique_ptr<AbilityManager>ability_;//能力

	int stateCnt_;		//キャラクター状態管理用カウンター
	int stateLimit_;	//状態をの時間上限
	int abilityBtnCnt_;	//能力使用ボタンの押下時間
	int operationImgs_[static_cast<int>(SceneManager::CNTL::MAX)];	//操作UIの画像
};

