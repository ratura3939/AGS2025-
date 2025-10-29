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
	static constexpr FLOAT4 NOMAL_COLOR = { 0.0f,0.0f,0.0f,1.0f };
	static constexpr float GRAVITY_POW = 0.98f;

	GimmickObjBase(void);
	virtual ~GimmickObjBase(void);

	void Init(void)override;
	virtual void Draw(void)override;
	void Release(void)override;

	/// <summary>
	/// 付与色の設定
	/// </summary>
	/// <param name="_color"></param>
	void SetObjectRenderColor(const FLOAT4& _color);

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


protected:
	virtual void SetParam(void) = 0;
	void UpdateRotQuat(void);

	void UpdateNomal(void);
	void UpdateAffectLock(void);
	void UpdateAffectMagnet(void);

	using Update_f = void(GimmickObjBase::*)(void);
	Update_f update_;

#pragma region shader関連
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> render_;
#pragma endregion

	float gravity_;	//重力
	bool isActiveGravity_;	//有効か無効か
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

