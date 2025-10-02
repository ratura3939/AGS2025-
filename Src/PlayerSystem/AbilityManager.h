#pragma once
#include<DxLib.h>
#include<string>

class StageManager;

class AbilityManager
{
public:
	/// <summary>
	/// 能力種類
	/// </summary>
	enum class ABILITY_TYPE {
		NONE=-1,
		MAGNET,
		LOCK_TIME,
		MAX
	};

	static const std::string UI_ABILITY_MGNET;
	static const std::string UI_ABILITY_LOCK_TIME;
	static const VECTOR ABILITY_ICON_POS;	//アイコン位置
	static constexpr float UI_EX = 0.25f;	//アイコン拡大率
	static constexpr int PRESSED_TIME_4_CHANGE_ABILITY = 50;	//能力切り換えフェーズまでに必要な押下時間

	AbilityManager(StageManager& _stage);
	~AbilityManager(void);

	void Update(void);
	void Draw(void);

	void RedyAbility(void);	//能力の開始(使用準備)
	void UseAbility(void);	//能力の使用
	void EndUsingAbility(void);	//能力の終了

	void ChangeAbility(const ABILITY_TYPE _type);	//能力切り換え

	const bool IsUsingAbility(void)const { return isUsingAbility_; }
	const ABILITY_TYPE GetNowAbility(void)const { return useAbility_; }
	std::string& GetAbilityUiList(void) { return *iconNames_; }

private:
	FLOAT4 GetAbilityColor(const ABILITY_TYPE _type);
	bool IsHitReticle(VECTOR _screenPos);

	StageManager& stage_;	//ステージ(参照)
	ABILITY_TYPE useAbility_;	//使用している能力

	std::string iconNames_[static_cast<int>(ABILITY_TYPE::MAX)];
	bool isUsingAbility_;	//能力使用中か
};

