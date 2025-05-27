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
	static constexpr int ENEMY_NUM = 1;			//“G‘”

	EnemyManager(void);
	~EnemyManager(void);

	void Init(void);
	void Update(const VECTOR& _playerPos);
	void Draw(void);
	void Release(void);

	//“G‚½‚¿‚Ìæ“¾
	std::vector<EnemyBase&> GetEnemys(void);

	//ˆÊ’uE‰ñ“]æ“¾
	const VECTOR GetPos(const int _num) { return characters_[_num]->GetPos(); }	//À•W
	const Quaternion GetQua(const int _num) { return characters_[_num]->GetQua(); }//‰ñ“]

	//ˆê”Ô‹ß‚¢“G‚Ì”z—ñ”Ô†‚ğ•Ô‚·
	int GetNearEnemyNum(const VECTOR _pPos);
	//‰æ–Ê“à‚Å‚ ‚é‚©‚Ç‚¤‚©
	bool InsideScreen(const VECTOR _pos);

	void DrawDebug(void);
private:
	std::vector<std::unique_ptr<EnemyBase>> characters_;
};

