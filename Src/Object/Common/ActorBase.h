#pragma once
#include<DxLib.h>
#include<memory>
#include"../../Common/Quaternion.h"
#include"Collider.h"

class Collider;

class ActorBase
{
public:
	ActorBase(void);
	virtual ~ActorBase(void);

	virtual void Init(void) = 0;
	void Update(void);
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	//位置設定
	void SetPos(const VECTOR& _pos);
	const VECTOR& GetPos(void)const;

	// 各種方向を取得
	const VECTOR& GetForward(void) const;
	const VECTOR& GetBack(void) const;
	const VECTOR& GetRight(void) const;
	const VECTOR& GetLeft(void) const;
	const VECTOR& GetUp(void) const;
	const VECTOR& GetDown(void) const;

	// 対象方向を取得
	const VECTOR& GetDir(const VECTOR& _vec) const;

	//衝突後の処理
	virtual void HitCollider(const Collider::MASTER_TYPE& _hitType) = 0;

protected:
	virtual void DoUpdate(void) = 0;	//派生クラスの更新処理

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

	std::unique_ptr<Collider> collider_;

private:
	void UpdateRotQuat(void);	//基礎情報の更新
};

