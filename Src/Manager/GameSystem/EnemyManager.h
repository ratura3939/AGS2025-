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
	static constexpr int ENEMY_NUM = 4;			//敵総数

	//攻撃登録名(ゆくゆくは外部データ)
	static const std::string ATTACK_NOMAL;

	//デバッグ用
	static constexpr int ATTACK_TIME = 100;
	static constexpr int ATTACK_TIME_START = 40;
	static constexpr int ATTACK_TIME_END = 90;

	static constexpr VECTOR INIT_1 = { 0.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_2 = { 500.0f,0.0f,1000.0f };
	static constexpr VECTOR INIT_3 = { 0.0f,0.0f,-1000.0f };
	static constexpr VECTOR INIT_4 = { -500.0f,0.0f,-1000.0f };

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

	/// <summary>
	/// 戦闘状態の敵が居るかどうか
	/// </summary>
	/// <returns>true=いる/false=いない</returns>
	bool IsBattleEnemy(void);

	/// <summary>
	/// IsBattleEnemyのONまたはOFFのトリガー
	/// </summary>
	/// <returns>true=トリガ起動/false=変化なし</returns>
	bool IsSwitchBattleOrNomalEnemyTrg(void);

	//アニメのスピードの設定
	void SetAnimSpeedRate(const float _percent);

	void DrawDebug(void);
private:
	std::vector<std::shared_ptr<EnemyBase>> characters_;
	bool preBattle_;	//位置フレーム前が戦闘状態であるかどうか
};

