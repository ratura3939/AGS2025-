#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/ActorBase.h"
#include "../../Common/Quaternion.h"
#include"../../Manager/GameSystem/AnimationController.h"

class AttackManager;
class ModelMaterial;
class ModelRenderer;
class Game;

class CharacterBase
	: public ActorBase
{
public:
	//初期化用
	static constexpr float INIT_MODEL_ROT = 180.0f;	//Unity形式のモデルの形を合わせる用

	//回転作業
	static constexpr float PER_ROT = 0.1f;			//フレームごとの回転(球面補間における時間の増加量を表す)
	static constexpr float THRESHOLD_ROT = 0.1f;	//回転のしきい値を表す

	//回転量
	static constexpr float DEG_FORWARD = 0.0f;	//前方
	static constexpr float DEG_BACK = 180.0f;	//後方
	static constexpr float DEG_LEFT = 270.0f;	//左方
	static constexpr float DEG_RIGHT = 90.0f;	//右方

	static constexpr VECTOR CHARACTER_HEIGHT = { 0.0f,150.0f,0.0f };	//キャラクターの身長
	static constexpr float CHARACTER_RADIUS = 50.0f;					//キャラクターの判定の大きさ

	//移動制限(大まか)
	static constexpr float MOVE_MAX = 5000.0f;

	//デバッグ用
	static constexpr int COLOR = 0xffffff;

	CharacterBase(void);
	virtual ~CharacterBase(void);

	void UpdateAnimOnly(void);
	virtual void Draw(void)override;
	void Release(void)override;

	//座標取得
	const VECTOR& GetHeight(void)const;			//頭辺り

	//当たり判定の半径
	const float GetCollisionRadius(void)const;

	//回転情報の取得
	const Quaternion& GetQua(void)const;
	void SetQua(const Quaternion _qua);

	//攻撃の当たり判定取得(マネージャ側で攻撃を登録する際に使用)
	std::weak_ptr<Collider> GetAttackCollider(void);

	//判定
	virtual const bool IsAlive(void)const;

	//ダメージを与える
	virtual void Damage(const float _pow) = 0;

	//死亡させる
	virtual void Deth(void);

	//当たり判定後処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

	//デバッグ
	virtual void DrawDebug(void);
	void DrawCupcel(void);

protected:
	virtual void DoInit(void)override = 0;
	virtual void DoUpdate(void)override = 0;
	virtual void DoHitCollider(std::weak_ptr<Collider>& _col) = 0;

	//必ず更新・初期化処理に入れること
	//************************************************
	virtual void InitAnim(void) = 0;
	virtual void InitUI(void) = 0;
	//**********************************************
	//UI描画
	virtual void DrawUI(void) = 0;

	//回転目標角度
	void SetGoalRot(const float _rad);
	//回転
	void Rotation(void);

#pragma region モデル基礎制御
	VECTOR headPos_; //頭辺りの座標
	VECTOR centerPos_; //モデル中央辺りの座標

	VECTOR uiPos_; //UI表示位置
	Quaternion characterRotY_;	//Y軸回転用

	Quaternion goalQua_;	//目標の回転量
	float stepRotation_;	//回転のカウンター
#pragma endregion

#pragma region 描画関連
	std::unique_ptr<ModelMaterial>material_;
	std::unique_ptr<ModelRenderer>renderer_;
#pragma endregion

	//アニメーション
	std::unique_ptr<AnimationController> animController_;

	std::shared_ptr<Collider> atkCollider_;	//攻撃用コライダー
	VECTOR atkPos_;	//攻撃位置

	//ステータスなど
	float hp_;
};

