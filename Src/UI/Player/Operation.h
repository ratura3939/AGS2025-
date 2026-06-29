#pragma once
#include "../UIBase.h"
#include"../../Manager/Generic/SceneManager.h"

class Operation :
    public UIBase
{
public:
	Operation(VECTOR& _followPos);
	~Operation(void)override;

	bool Init(const std::string& _master)override;
	bool Update(void)override;
	void Draw(void)override;

	void Reset(void)override;

	void SetController(const SceneManager::CNTL& _cntl);

private:
	std::string buttonOperationStr_;	//アクション対応ボタン
	std::string menuOperationStr_;		//メニュー対応ボタン
	std::string abilityOperationStr_;	//アビリティ対応ボタン

	int operationImgs_[static_cast<int>(SceneManager::CNTL::MAX)];	//操作UIの画像
	int toMenuOperationImgs_[static_cast<int>(SceneManager::CNTL::MAX)];	//メニュー遷移UIの画像
	int abilityOperationImgs_[static_cast<int>(SceneManager::CNTL::MAX)];	//メニュー遷移UIの画像
};

