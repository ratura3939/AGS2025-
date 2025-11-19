#pragma once
#include<DxLib.h>
#include<memory>
#include"../../Common/ActorBase.h"
#include"../../../Common/Quaternion.h"

class ModelMaterial;
class ModelRenderer;

class GimmickObjBase 
	: public ActorBase
{
public:
	static constexpr float UNITY_MODEL_ADJUST_Y = 1500.0f;
	static constexpr FLOAT4 NOMAL_COLOR = { 0.0f,0.0f,0.0f,1.0f };
	static constexpr float MOVE_SPEED_DEC = -1.0f;

	GimmickObjBase(void);
	virtual ~GimmickObjBase(void);

	virtual void Draw(void)override;
	virtual void DrawDebug(void);
	void Release(void)override;

	/// <summary>
	/// 付与色の設定
	/// </summary>
	/// <param name="_color"></param>
	virtual void SetObjectRenderColor(const FLOAT4& _color);

	//位置設定
	const VECTOR& GetScreenPos(void)const;

	//モデル取得
	const int GetModelId(void)const;

	//能力の影響を受けるか
	const bool IsAffectAbility(void)const;	

	//影響を受けているか
	void SetIsAffecting(const bool _flag);
	const bool IsAffecting(void)const;

	//影響開始
	void AffectedLockTime(void);
	void AffectedMagnet(void);
	void FinishAffect(void);

	void ResetGravity(void) { gravity_ = { 0.0f,0.0f,0.0f }; }

protected:
	void DoInit(void)override;
	virtual void SetParam(void) = 0;

	virtual void UpdateNomal(void);
	void UpdateAffectLock(void);
	void UpdateAffectMagnet(void);


	void DecMoveSpeed(void);	//移動量減衰処理

	using Update_f = void(GimmickObjBase::*)(void);
	Update_f update_;

#pragma region shader関連
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> render_;
#pragma endregion

	VECTOR moveDir_;	//移動方向ベクトル
	float moveSpeed_;	//移動速度
	bool isAffectAbilyty_;	//能力の影響を受けるか
	bool isAffectingNow_;	//現在影響を受けているか
	bool isTargeting_;		//対象として選択されているか

	VECTOR screenPos_;		//スクリーン上での位置

	//デバッグ
	bool isDrawScreenPosCircle_;
	int screenPosColor_;

private:
	void DoUpdate(void)override;
};

