#pragma once
#include<DxLib.h>
#include "../../Common/Quaternion.h"
class CharacterBase
{
public:

	static constexpr float TIME_ROT = 5.0f;

	void Init(void);
	virtual void Update(void) = 0;
	virtual void Draw(void);
	virtual void Release(void);

	// 各種方向を取得
	VECTOR GetForward(void) const;
	VECTOR GetBack(void) const;
	VECTOR GetRight(void) const;
	VECTOR GetLeft(void) const;
	VECTOR GetUp(void) const;
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& _vec) const;

	//位置取得
	const VECTOR GetPos(void)const;
	//回転情報の取得
	const Quaternion GetQua(void)const;

	virtual void DrawDebug(void);
protected:
	//固有データ書き出し用
	virtual void SetPram(void);

	int modelId_;	//モデルID
	VECTOR pos_;	//座標
	VECTOR scl_;	//モデル大きさ
	VECTOR rot_;	//回転情報(XYZ)

	Quaternion startQua_;	//開始時の回転量
	Quaternion goalQua_;	//目標の回転量
	float stepRotation_;	//回転のカウンター

	//各情報の行列か
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;

	// ローカル回転
	Quaternion quaRotLocal_;

	//必ず更新処理に入れること
	//************************************************
	void UpdateRotQuat(void);
	//**********************************************

	//回転目標角度
	void SetGoalRot(const float _rad);
	//回転
	void Rotation(void);
};

