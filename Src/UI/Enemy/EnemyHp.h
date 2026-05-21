#pragma once
#include"../UIBase.h"

class EnemyHp :
	public UIBase
{
public:
	static constexpr VECTOR HP_UI_START_POS = { -50.0f,0.0f,0.0f };	//Hpカプセルの始点の相対座標
	static constexpr float HP_MAX_PERCENT = 100.0f;		//最大の割合

	EnemyHp(VECTOR& _followPos,float& _now, float _max);
	~EnemyHp(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	float& nowHp_;	//現在のHP
	float hpMax_;	//最大HP
};

