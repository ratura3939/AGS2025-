#pragma once
#include "../UIBase.h"
#include<string>

class EnemyCount :
    public UIBase
{
public:
	EnemyCount(VECTOR& _followPos);
	~EnemyCount(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

	void SetNumImg(int _img);
	void SetBossIconImg(void);
	
private:
	std::string plateStr_;
	std::string numberStr_;
	std::string skeltonConterStr_;

	int golemIcon_;
};

