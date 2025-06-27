#pragma once
#include"../UIBase.h"

class EnemyHp :
	public UIBase
{
public:
	EnemyHp(void);
	~EnemyHp(void);

	bool Init(void)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	int nowHp_;
	int hpMax_;
};

