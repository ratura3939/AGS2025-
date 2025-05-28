#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class Game;
class PlayerChara;
class AttackManager;

class PlayerManager
{
public:
	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;

	//デバッグ用
	static constexpr int ATTACK_TIME = 50;

	PlayerManager(Game& _gameScene);
	~PlayerManager(void);

	void Init(void);
	void Update(AttackManager& _atk);
	void Draw(void);
	void Release(void);

	//キャラクター取得
	std::weak_ptr<PlayerChara> GetPlayer(void);

	//位置・回転取得
	const VECTOR GetPos(void);			//座標
	const Quaternion GetQua(void);		//回転
	const VECTOR GetFocusPoint(void);	//注視点

	//ロックオンのトリガ
	//デバッグではただプレイヤーが持っている感を出しているだけ
	const bool IsRockOnTrg(void)const;
	const bool IsRockOffTrg(void)const;

	const bool IsAlive(void)const;

	//ロックオン・オフ時に必要な処理
	void RockOn(void);
	void RockOff(void);

	void DrawDebug(void);
private:
	Game& scene_;	//ゲームクラス参照
	std::shared_ptr<PlayerChara> character_;
};

