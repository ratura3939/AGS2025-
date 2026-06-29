#pragma once
#include "../UIBase.h"
class Operation :
    public UIBase
{
public:
	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

private:
	std::string buttonOperationStr_;	//アクション対応ボタン
	std::string menuOperationStr_;		//メニュー対応ボタン
};

