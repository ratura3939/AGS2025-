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
	static const std::string ATTACK_NOMAL;

	PlayerManager(Game& _gameScene);
	~PlayerManager(void);

	void Init(void);
	void Update(AttackManager& _atk);
	void Draw(void);
	void Release(void);

	const VECTOR GetPos(void);
	const Quaternion GetQua(void);
	const VECTOR GetFocusPoint(void);

	//ロックオンのトリガ
	//デバッグではただプレイヤーが持っている感を出しているだけ
	const bool IsRockOnTrg(void)const;
	const bool IsRockOffTrg(void)const;

	//ロックオン・オフ時に必要な処理
	void RockOn(void);
	void RockOff(void);

	void DrawDebug(void);
private:
	Game& scene_;	//ゲームクラス参照
	std::unique_ptr<PlayerChara> character_;
};

