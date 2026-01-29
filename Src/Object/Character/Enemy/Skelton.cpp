#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Collider.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include "Skelton.h"

namespace {
#pragma region アニメーション関連
    const int ANIM_IDLE = 42;    //待機

    //攻撃(継承先ごとで違うので最終的にはここではない)
    const int ANIM_ATTACK_NOMAL = 9;     //攻撃

      //その他アクション
    const int ANIM_WALK = 94;				//歩き
    const int ANIM_DUSH_FORWARD = 55;		//ダッシュ(前方)
    const int CHARGE_ATTACK = 84;			//攻撃チャージ
	const int HIT_DAMAGE = 39;				//被ダメ

    //演出
    const int ANIM_DETH_START = 29;      //死亡開始
    const int ANIM_DETH_SUSTANABLE = 28; //死亡持続
#pragma endregion

	const float UI_DEVIATION_Y = 300.0f* EnemyBase::CHARA_SCALE; //UI表示位置補正Y
	const int ATK_CHARGE_CNT_MAX = 40; //攻撃チャージ最大値
}

Skelton::Skelton(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos)
	:EnemyBase(_pos,_num,_atk,_pPos)
{
}

Skelton::~Skelton(void)
{
}

void Skelton::InitAnim(void)
{
    animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("attack", ANIM_ATTACK_NOMAL, AnimationController::PLAY_TYPE::NOMAL,true);
    animController_->Add("charge", CHARGE_ATTACK, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("hitDamage", HIT_DAMAGE, AnimationController::PLAY_TYPE::NOMAL,true);
    animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dush", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
    animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
    animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void Skelton::SetParam(void)
{
	//各敵たち
	//後々Jsonやったら楽になるかも？
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);

	if (modelId_ == -1) {
		return;
	}
	//パラメータ関係
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);

	//当たり判定大きさ
	colRadius_ = CHARACTER_RADIUS;
	//当たり判定
	headPos_ = VAdd(pos_, CHARACTER_HEIGHT);	//頭位置
	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::ENEMY},
		std::move(std::make_unique<Capsule>(headPos_, pos_, colRadius_)));

	//攻撃の発生位置(相対座標)
	atkRelative_ = RELATIVE_ATTACK_POS;

	//攻撃
	atkPos_ = VAdd(pos_, atkRelative_);
	atkCollider_ = std::make_shared<Collider>(*this, std::set<TAG>{ TAG::ENEMY, TAG::ATTACK },
		std::move(std::make_unique<Sphere>(atkPos_, SCALE_ATTACK_NOMAL))
	, std::set<TAG>{TAG::STAGE, TAG::OBJECT});

	atkCollider_->SetUseThis(false);

	CollisionManager::GetInstance().AddCollider(atkCollider_);	//当たり判定登録

	//攻撃可能距離
	atkDistance_ = ATTACK_DISTANCE;
	//攻撃チャージ関連
	atkChargeCntMax_ = ATK_CHARGE_CNT_MAX;	//攻撃が一種類のみなのでここに

	//アニメーション初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	uiDeviationY_ = UI_DEVIATION_Y;
	maxHp_ = ENEMY_HP;

	//位置設定
	uiPos_ = pos_;
	//頭位置
	uiPos_.y += uiDeviationY_;

	//UI初期化
	InitUI();

	//モデル描画クラス生成
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS


	//PS
	//各色の強さ(拡散光)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//ブラーの強さ(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//サンプル数(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });

	intervalCnt_ = 0.0f;

	//状態を通常に
	ChangeState(ENEMY_STATE::NOMAL);
}

void Skelton::UpdateBattle(void)
{
	auto& efcM = EffectManager::GetInstance();
	float distance = Utility::MagnitudeF(VSub(pPos_, pos_));

	//カウンタ増加(ゲーム更新スピード)
	intervalCnt_ += SceneManager::GetInstance().GetUpdateSpeedRate_();

	//移動処理
	if (!isStay_) {
		if (distance >= ATTACK_DISTANCE) {
			(this->*move_)(pPos_);
		}
	}
	else {
		//攻撃チャージ中
		OderGoalRot(pPos_);	//回転の設定だけは行う

		atkChargeCnt_++;
		//一定時間経過したら
		if (atkChargeCntMax_ < atkChargeCnt_) {
			//攻撃する
			atkManager_.Attack(speciesName_, "SwingSword");
			animController_->Play("attack", SPEED_ANIM);
			stopTime_ = atkManager_.GetTotalTime(EnemyManager::ATTACK_NOMAL);
			intervalCnt_ = 0.0f;
			atkChargeCnt_ = 0;
			isStay_ = false;
		}
		return;
	}

	//判定
	//プレイヤーが戦闘状態範囲度外にでたら
	if (distance > BATTLE_FINISH_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//プレイヤーが攻撃範囲内かつ攻撃可能な間隔を開けているのなら
	if (distance <= ATTACK_DISTANCE && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		const float ChargeAtkEfcScale = 40.0f;
		const float ChargeAtkEfcSpeed = 0.4f;

		//攻撃チャージ
		efcM.Play(GetSpeciesName(), "Charge", centerPos_, rot_, ChargeAtkEfcScale, ChargeAtkEfcSpeed);
		animController_->Play("charge", SPEED_ANIM);
		//攻撃チャージ中のためステイに
		isStay_ = true;
		atkChargeCnt_ = 0;
	}
}

void Skelton::DamageReaction(void)
{
	//被ダメアニメーション
	animController_->Play("hitDamage", SPEED_ANIM);

	//バトル中
	if (state_ == ENEMY_STATE::BATTLE) {
		//攻撃チャージ中かもしれないのでそれら初期化
		isStay_ = false;
		atkChargeCnt_ = 0;
		intervalCnt_ = 0.0f;
		//可能性のあるエフェクトの停止
		EffectManager::GetInstance().Stop(speciesName_, "Charge");
	}
}
