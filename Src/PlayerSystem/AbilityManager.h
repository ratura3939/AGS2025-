#pragma once
#include<DxLib.h>
#include<string>
#include"../Common/Quaternion.h"
#include"../Object/Stage/StageObj/GimmickObjBase.h"

class StageManager;
class PlayerChara;
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

	//定数
	static const std::string UI_ABILITY_MGNET;		//マグネットUI登録名
	static const std::string UI_ABILITY_LOCK_TIME;	//ロックタイムUI登録名
	static const VECTOR RETICLE_POS;		//照準位置
	static const VECTOR ABILITY_ICON_POS;	//アイコン位置
	static constexpr float ABILITY_UI_EX = 0.25f;	//アイコン拡大率
	static constexpr int PRESSED_TIME_4_CHANGE_ABILITY = 50;	//能力切り換えフェーズまでに必要な押下時間

	AbilityManager(StageManager& _stage, PlayerChara& _master);
	~AbilityManager(void);

	void Update(void);	//更新
	void Draw(void);	//描画

	void ChangeAbility(const ABILITY_TYPE _type);	//能力切り換え

	const bool IsHitReticle(void)const { return !selectObj_.expired(); }				//レティクルに当たっているか(selectObj_に中身があるということはレティクルに当たっているということ)
	[[nodiscard]] const ABILITY_TYPE GetNowAbility(void)const { return useAbility_; }	//現在アビリティを使用しているか
	[[nodiscard]] const STATE GetAbilityState(void)const { return state_; }				//アビリティの使用状況の取得
	std::string& GetAbilityUiList(void) { return *iconNames_; }							//使用アビリティのUI登録名取得
	const bool IsUseMagnet(void) { return state_ == STATE::USE && useAbility_ == ABILITY_TYPE::MAGNET; }	//マグネット使用中か(マグネット使用中の注視点は少し違くなる)
	[[nodiscard]] const VECTOR GetFollowPosForUseMagnet(const VECTOR _playerPos);		//マグネット使用時の注視点を返す

	void ChangeState(const STATE _next);									//状態の変更
	const bool IsPlayerAnyInput(void)const { return isPlayerAnyInput_; }	//プレイヤ－の入力を受け付けるか

private:
	FLOAT4 GetAbilityColor(const ABILITY_TYPE _type);	//使用アビリティの固有色の取得
	bool IsHitReticle(const VECTOR& _screenPos);		//レティクルとの衝突判定
	bool IsNearObject2Camera(const VECTOR& _pos1, const VECTOR& _pos2);	//距離比較(カメラとの距離)

	void RedyAbility(void);			//能力の開始(使用準備)
	void DirectionAbility(void);	//能力の開始(演出)
	void UseAbility(void);			//能力の使用
	void EndUsingAbility(void);		//能力の終了

	void UpdateRedy(void);		//準備時更新
	void UpdateDirection(void);	//演出時更新
	void UpdateUse(void);		//使用時更新
	void UpdateEnd(void);		//終了時更新

	PlayerChara& master_;		//使用者(参照)
	StageManager& stage_;		//ステージ(参照)
	ABILITY_TYPE useAbility_;	//使用している能力
	STATE state_;				//能力発動ロジックの段階
	std::unique_ptr<AbilityBase> abilities_[static_cast<int>(ABILITY_TYPE::MAX)];	//能力

	std::string iconNames_[static_cast<int>(ABILITY_TYPE::MAX)];	//アイコン画像
	std::weak_ptr<GimmickObjBase> selectObj_;	//選択中の物
	FLOAT4 selectColores_[static_cast<int>(ABILITY_TYPE::MAX)];
	bool isPlayerAnyInput_;	//プレイヤーの攻撃などの入力を許可するか

	using Update_f = void(AbilityManager::*)(void);
	Update_f update_;
};

