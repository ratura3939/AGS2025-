#pragma once
#include "../SceneBase.h"

#include<string>
#include"../../PlayerSystem/AbilityManager.h"

class SelectAbility :
    public SceneBase
{
public:
	SelectAbility(AbilityManager& _ability);
	~SelectAbility(void)override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;
	void LerpSelect(void);	//選択項目の線形保管

	void ChangeSelectAbility(const int _prev, const int _next);
	void ResetUiInfo(void);

	AbilityManager& ability_;	//能力マネージャ
	AbilityManager::ABILITY_TYPE selectAbility_;	//選択される能力
	std::string iconNames_[static_cast<int>(AbilityManager::ABILITY_TYPE::MAX)];

#pragma region 項目切り換えに置ける表現に使用する変数
	bool isChangeSelect_;	//項目が切り替えが行われているか
	float changeStep_;		//能力の項目切り換えの線形補完用

	std::string nextSelect_;	//非選択→選択になるもの
	std::string prevSelect_;	//選択→非選択になるもの
#pragma endregion
};

