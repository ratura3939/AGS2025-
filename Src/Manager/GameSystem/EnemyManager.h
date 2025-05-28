#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;
class Game;

class EnemyManager
{
public:
	static constexpr int ENEMY_NUM = 1;			//敵総数

	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;

	//デバッグ用
	static constexpr int ATTACK_TIME = 50;

	EnemyManager(void);
	~EnemyManager(void);

	void Init(void);
	void Update(const VECTOR& _playerPos, AttackManager& _atkMng);
	void Draw(void);
	void Release(void);

	//敵たちの取得
	std::vector<std::weak_ptr<EnemyBase>> GetEnemys(void);

	//位置・回転取得
	const VECTOR GetPos(const int _num);//座標
	const Quaternion GetQua(const int _num);//回転

	//一番近い敵の配列番号を返す
	int GetNearEnemyNum(const VECTOR _pPos);
	//画面内であるかどうか
	bool InsideScreen(const VECTOR _pos);

	void DrawDebug(void);
private:
	std::vector<std::shared_ptr<EnemyBase>> characters_;
};

