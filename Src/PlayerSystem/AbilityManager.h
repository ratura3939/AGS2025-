#pragma once
#include<DxLib.h>
#include<string>
#include"../Object/Stage/StageObj/GimmickObjBase.h"

class StageManager;
class AbilityBase;

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

	/// <summary>
	/// 能力使用フロー
	/// </summary>
	enum class STATE {
		REDY,		//準備
		DIRECTION,	//演出
		USE,		//使用
		END			//終了
	};

	static const std::string UI_ABILITY_MGNET;
	static const std::string UI_ABILITY_LOCK_TIME;
	static const VECTOR RETICLE_POS;		//照準位置
	static const VECTOR ABILITY_ICON_POS;	//アイコン位置
	static constexpr float UI_EX = 0.25f;	//アイコン拡大率
	static constexpr int PRESSED_TIME_4_CHANGE_ABILITY = 50;	//能力切り換えフェーズまでに必要な押下時間

	AbilityManager(StageManager& _stage);
	~AbilityManager(void);

	void Update(const VECTOR _playerPos);
	void Draw(void);

	void RedyAbility(void);	//能力の開始(使用準備)
	void UseAbility(void);	//能力の使用(演出→使用)
	void EndUsingAbility(void);	//能力の終了

	void DoUse(void);	//能力の実際の使用

	void ChangeAbility(const ABILITY_TYPE _type);	//能力切り換え

	const bool IsRedyAbility(void)const { return isRedyAbility_; }
	const bool IsUsingAbility(void)const { return isUsingAbility_; }

	const bool IsHitReticle(void)const { return !selectObj_.expired(); }	//レティクルに当たっているか(selectObj_に中身があるということはレティクルに当たっているということ)
	const ABILITY_TYPE GetNowAbility(void)const { return useAbility_; }
	std::string& GetAbilityUiList(void) { return *iconNames_; }

	const STATE GetAbilityState(void)const { return state_; }

private:
	FLOAT4 GetAbilityColor(const ABILITY_TYPE _type);
	bool IsHitReticle(VECTOR _screenPos);
	bool IsNearObject2Camera(const VECTOR _pos1, const VECTOR _pos2);

	void UpdateRedy(const VECTOR _playerPos);
	void UpdateDirection(const VECTOR _playerPos);
	void UpdateUse(const VECTOR _playerPos);
	void UpdateEnd(const VECTOR _playerPos);

	void ChangeState(const STATE _next);

	StageManager& stage_;		//ステージ(参照)
	ABILITY_TYPE useAbility_;	//使用している能力
	STATE state_;				//能力発動ロジックの段階
	std::unique_ptr<AbilityBase> abilities_[static_cast<int>(ABILITY_TYPE::MAX)];	//能力

	std::string iconNames_[static_cast<int>(ABILITY_TYPE::MAX)];	//アイコン画像
	bool isRedyAbility_;	//能力準備中か
	bool isUsingAbility_;	//能力使用中か
	std::weak_ptr<GimmickObjBase> selectObj_;	//選択中の物
	FLOAT4 selectColores_[static_cast<int>(ABILITY_TYPE::MAX)];

	using Update_f = void(AbilityManager::*)(const VECTOR _playerPos);
	Update_f update_;
};

