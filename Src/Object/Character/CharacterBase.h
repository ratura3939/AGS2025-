#pragma once
#include<DxLib.h>
#include "../../Common/Quaternion.h"

class Game;

class CharacterBase
{
public:
	//初期化用
	static constexpr float INIT_MODEL_ROT = 180.0f;	//Unity形式のモデルの形を合わせる用

	//回転作業
	static constexpr float TIME_ROT = 5.0f;
	static constexpr float PER_ROT = 0.05f;			//フレームごとの回転(球面補間における時間の増加量を表す)
	static constexpr float THRESHOLD_ROT = 0.1f;	//回転のしきい値を表す

	//回転量
	static constexpr float DEG_FORWARD = 0.0f;
	static constexpr float DEG_BACK = 180.0f;
	static constexpr float DEG_LEFT = 270.0f;
	static constexpr float DEG_RIGHT = 90.0f;

	CharacterBase(void);
	~CharacterBase(void);

	virtual const bool Init(void) = 0;
	virtual void Update(void);
	virtual void Update(const VECTOR _pPos);
	virtual void Draw(void);
	virtual const bool Release(void);

protected:
	//必ず更新・初期化処理に入れること
	//************************************************
	void UpdateRotQuat(void);
	//**********************************************
	//回転目標角度
	void SetGoalRot(const float _rad);
	//回転
	void Rotation(void);

public:
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
	int modelId_;	//モデルID
	VECTOR pos_;	//座標
	VECTOR scl_;	//モデル大きさ
	VECTOR rot_;	//回転情報(XYZ)
	Quaternion characterRotY_;	//Y軸回転用

	Quaternion goalQua_;	//目標の回転量
	float stepRotation_;	//回転のカウンター

	//各情報の行列か
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;
	Quaternion quaRotOrigin_;

	// ローカル回転
	Quaternion quaRotLocal_;

	//ステータスなど
	float hp_;
};

