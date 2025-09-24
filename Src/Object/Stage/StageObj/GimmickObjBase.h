#pragma once

#include<DxLib.h>
#include<memory>
#include"../../../Common/Quaternion.h"

class ModelMaterial;
class ModelRenderer;

class GimmickObjBase
{
public:
	GimmickObjBase(void);
	virtual ~GimmickObjBase(void);

	void Init(void);
	void Update(void);
	virtual void Draw(void);
	void Release(void);

	/// <summary>
	/// 付与色の設定
	/// </summary>
	/// <param name="_color"></param>
	void SetObjectRenderColor(const FLOAT4 _color);

	void SetPos(const VECTOR _pos);

protected:
	virtual void SetPram(void) = 0;
	void UpdateRotQuat(void);

#pragma region モデル基礎
	int modelId_;	//モデルID

	VECTOR pos_;	//座標
	VECTOR scl_;	//モデル大きさ
	VECTOR rot_;	//回転情報(XYZ)

	//各情報の行列か
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ローカル回転
	Quaternion quaRotLocal_;
#pragma endregion

	float gravity_;	//重力

#pragma region shader関連
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> render_;

	FLOAT4 objColor_;
#pragma endregion

private:

};

