#pragma once

class StageManager;

class AbilityManager
{
public:
	/// <summary>
	/// 能力種類
	/// </summary>
	enum class ABILITY_TYPE {
		NONE,
		MAGNET,
		TIME_LOCK
	};

	AbilityManager(StageManager& _stage);
	~AbilityManager(void);

	void Update(void);

	void RedyAbility(void);	//能力の開始(使用準備)
	void UseAbility(void);	//能力の使用
	void EndUsingAbility(void);	//能力の終了

	void ChangeAbility(void);	//能力切り換え

	const bool IsUsingAbility(void) { return isUsingAbility_; }

private:
	VECTOR GetAbilityColor(const ABILITY_TYPE _type);

	StageManager& stage_;	//ステージ(参照)
	ABILITY_TYPE useAbility_;	//使用している能力

	bool isUsingAbility_;	//能力使用中か
};

