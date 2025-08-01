#pragma once
#include "../UIBase.h"
#include<string>

class EnemyCount :
    public UIBase
{
public:
	enum class COUNTER_STATE {
		NOAML,
		ACTION,
	};

	EnemyCount(VECTOR& _followPos);
	~EnemyCount(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

	void SetNumImg(int _img);
	void SetIconImg(void);
	void Action(void);
	
private:
	void StartAction(void);
	void NomalUpdate(void);		//通常更新
	void ActionUpdate(void);	//敵減少時演出用

	void UpdateMove(void);		//目標位置へ移動用の関数(スケール拡大も行う)
	void UpdateDirection(void);	//減らした時に揺らすよう

	std::string plateStr_;
	std::string numberStr_;
	std::string SkeltonConterStr_;
	COUNTER_STATE state_;

	using Update_f = void(EnemyCount::*)(void);
	Update_f update_;
	Update_f actionUpdate_;

	int golemIcon_;
};

