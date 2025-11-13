#pragma once
#include<DxLib.h>
#include<memory>
#include"../../Common/Quaternion.h"
#include"Collider.h"

class ActorBase
{
public:
	//重力定数
	static constexpr float GRAVITY_POW = -0.98f;

	ActorBase(void);
	virtual ~ActorBase(void);

	void Init(void);
	void Update(void);
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	//位置設定
	void SetPos(const VECTOR& _pos);
	void SetPrevPos(void);
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
	virtual void HitCollider(std::weak_ptr<Collider> _col) = 0;

	//攻撃力の取得
	const float GetPower(void)const;	

	//個体名取得
	const std::string& GetSpeciesName(void)const;

protected:
	virtual void DoInit(void) = 0;
	virtual void DoUpdate(void) = 0;	//派生クラスの更新処理

#pragma region モデル基礎
	int modelId_;	//モデルID

	VECTOR pos_;	//座標
	VECTOR prevPos_; //前フレームの座標
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

	//個体名
	std::string speciesName_;

	std::shared_ptr<Collider> collider_;
	float power_;	//攻撃力

	VECTOR gravity_; //重力ベクトル
	bool isActiveGravity_; //重力が有効か

private:
	void UpdateRotQuat(void);	//基礎情報の更新
	void UpdateGravity(void);	//重力処理
};

