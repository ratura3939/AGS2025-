#pragma once
#include<DxLib.h>
#include<string>
#include"../Common/Quaternion.h"
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

	void Update(const VECTOR _playerPos,const Quaternion _playerQua);
	void Draw(void);

	void ChangeAbility(const ABILITY_TYPE _type);	//能力切り換え

	const bool IsHitReticle(void)const { return !selectObj_.expired(); }	//レティクルに当たっているか(selectObj_に中身があるということはレティクルに当たっているということ)
	[[nodiscard]] const ABILITY_TYPE GetNowAbility(void)const { return useAbility_; }
	[[nodiscard]] const STATE GetAbilityState(void)const { return state_; }
	std::string& GetAbilityUiList(void) { return *iconNames_; }
	const bool IsUseMagnet(void) { return state_ == STATE::USE && useAbility_ == ABILITY_TYPE::MAGNET; }	//マグネット使用中か(マグネット使用中の注視点は少し違くなる)
	[[nodiscard]] const VECTOR GetFollowPos4UseMagnet(const VECTOR _playerPos);

	void ChangeState(const STATE _next);

private:
	FLOAT4 GetAbilityColor(const ABILITY_TYPE _type);
	bool IsHitReticle(VECTOR _screenPos);
	bool IsNearObject2Camera(const VECTOR _pos1, const VECTOR _pos2);

	void RedyAbility(void);			//能力の開始(使用準備)
	void DirectionAbility(void);	//能力の開始(演出)
	void UseAbility(void);			//能力の使用
	void EndUsingAbility(void);		//能力の終了

	void UpdateRedy(const VECTOR _playerPos, const Quaternion _playerQua);
	void UpdateDirection(const VECTOR _playerPos, const Quaternion _playerQua);
	void UpdateUse(const VECTOR _playerPos, const Quaternion _playerQua);
	void UpdateEnd(const VECTOR _playerPos, const Quaternion _playerQua);

	StageManager& stage_;		//ステージ(参照)
	ABILITY_TYPE useAbility_;	//使用している能力
	STATE state_;				//能力発動ロジックの段階
	std::unique_ptr<AbilityBase> abilities_[static_cast<int>(ABILITY_TYPE::MAX)];	//能力

	std::string iconNames_[static_cast<int>(ABILITY_TYPE::MAX)];	//アイコン画像
	std::weak_ptr<GimmickObjBase> selectObj_;	//選択中の物
	FLOAT4 selectColores_[static_cast<int>(ABILITY_TYPE::MAX)];

	using Update_f = void(AbilityManager::*)(const VECTOR _playerPos, const Quaternion _playerQua);
	Update_f update_;

	VECTOR test_;
};

