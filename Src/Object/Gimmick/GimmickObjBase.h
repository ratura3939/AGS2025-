#pragma once

#include<DxLib.h>
#include"../../Common/Quaternion.h"

class GimmickObjBase
{
public:
	GimmickObjBase(void);
	virtual ~GimmickObjBase(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

protected:
	virtual void SetPram(void) = 0;
	void UpdateRotQuat(void);

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

	float gravity_;	//重力

private:

};

