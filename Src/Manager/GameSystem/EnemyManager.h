#pragma once
#include<memory>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;

class EnemyManager
{
public:
	EnemyManager(void);
	~EnemyManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const VECTOR GetPos(void);
	const Quaternion GetQua(void);

	void DrawDebug(void);
private:
	std::unique_ptr<EnemyBase> character_;
};

