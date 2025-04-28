#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;

class EnemyManager
{
public:

	static constexpr int ENEMY_NUM = 1;

	EnemyManager(void);
	~EnemyManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const VECTOR GetPos(const int _num);
	const Quaternion GetQua(const int _num);

	//ˆê”Ô‹ß‚¢“G‚Ì”z—ñ”Ô†‚ğ•Ô‚·
	int GetNearEnemyNum(const VECTOR _pPos);
	//‰æ–Ê“à‚Å‚ ‚é‚©‚Ç‚¤‚©
	bool InsideScreen(const VECTOR _pos);

	void DrawDebug(void);
private:
	std::vector<std::unique_ptr<EnemyBase>> characters_;
};

