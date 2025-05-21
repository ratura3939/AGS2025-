#pragma once
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class Arrow
{
public:

	//定数
	static constexpr VECTOR ARROW_LOCAL_POS = { 15.0f,30.0f,50.0f };	//打ち始めに違和感がないように調整
	static constexpr float GRAVITY = 0.5f;			//重力
	static constexpr float START_UP_ANGLE = 10.0f;	//最初の上アングル

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	//モデルID、発生位置、行き先,方向,対象位置,攻撃力
	Arrow(void);	
	~Arrow(void) = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_mdlId">矢のモデルID</param>
	/// <param name="_trans">アーチャーの位置情報等</param>
	/// <param name="_speed">速度</param>
	void Init(const int _mdlId, const float _speed);
	void Update(void);
	void Draw(void);
	void Release();
	void Destroy(void);

private:
	void UpdateRotQuat(void);
	//移動処理
	void Move(void);
	//生存フラグ設定
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }

public:
	const VECTOR GetPos(void)const { return pos_; }
	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

	//状況変位
	void ChangeState(const STATE _state) { state_ = _state; }

private:
	// 各種方向を取得
	VECTOR GetForward(void) const;
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(const VECTOR& _vec) const;

private:
	//位置情報
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


	//状態
	STATE state_;
	//攻撃力
	float atkPow_;
	//速度
	float speed_;
	//生存判定
	bool isAlive_;

	
};

