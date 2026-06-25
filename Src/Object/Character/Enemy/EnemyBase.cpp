#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../UI/EnemyUIController.h"
#include"../../../Utility/Utility.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include"../../Common/Collider.h"
#include"../../Common/Geometry/Capsule.h"
#include "EnemyBase.h"

namespace {
	const VECTOR forward={0.0f,0.0f,100.0f};
}

EnemyBase::EnemyBase(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos)
	:CharacterBase()
	,atkManager_(_atk)
	,pPos_(_pPos)
	,update_(&EnemyBase::UpdateNomal)
	,move_(&EnemyBase::MoveNomal)
	,uiCntl_(nullptr)
	,maxHp_(ENEMY_HP)
	,preStayPos_(Utility::VECTOR_INIT)
	,uiDeviationY_(-1.0f)
	,moveOneTime_(-1.0f)
	,moveSpeed_(MOVE_POW)
	,isStay_(true)
	,stayCnt_(STAY_TIME)
	,stopTime_(-1.0f)
	,intervalCnt_(INTERVAL_ATTACK_NOMAL)
	,searchRestartCnt_(0.0f)
	,searchCnt_(0.0f)
	,atkChargeCnt_(0)
	,atkChargeCntMax_(0)
	,isAlive_(true)
	,state_(ENEMY_STATE::MAX)
	,isLockTarget_(false)
	,atkRelative_(Utility::VECTOR_INIT)
	,atkDistance_(-1.0f)
	,colRadius_(-1.0f)
{
	speciesName_ = "Enemy" + std::to_string(_num);
	pos_ = _pos;
	uiPos_ = Utility::VECTOR_INIT;
	hp_ = ENEMY_HP;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::DoInit(void)
{
	SetParam();	//パラメータ設定

	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録
	power_ = POW_ATTACK_NOMAL;		//攻撃力
	renderer_ = std::make_unique<ModelRenderer>(modelId_, *material_);	//レンダラー
}


void EnemyBase::DoUpdate(void)
{
	headPos_ = VAdd(pos_, CHARACTER_HEIGHT);	//頭位置
	atkPos_ = VAdd(pos_, characterRotY_.PosAxis(atkRelative_));	//攻撃発生位置
	centerPos_ = headPos_;	//モデル中央
	centerPos_.y /= HALF;

	//行動更新
	(this->*update_)();
	//共通更新
	Rotation();

	animController_->Update();
	//位置設定
	uiPos_ = pos_;
	//頭位置
	uiPos_.y = uiDeviationY_;

	uiCntl_->Update();	//UI更新
}

void EnemyBase::SetNextGoalPos(void)
{
	//行先の角度設定
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
	float degRand = static_cast<float>(GetRand(static_cast<int>(Utility::CIRCLE_DEG)));
	float radRand = Utility::Deg2RadF(degRand) - cameraRot.y;
	//クォータニオンに変換
	Quaternion quaGoal = Quaternion::AngleAxis((double)cameraRot.y + radRand, Utility::AXIS_Y);
	//キャラクター回転も設定
	SetGoalRot(radRand);
	characterRotY_ = quaGoal;

	//移動量を範囲付きのランダムで生成
	moveOneTime_ = static_cast<float>(GetRand(static_cast<int>(MOVE_RANDOM_MAX))) + MOVE_RANDOM_MIN;
	//ステイ状態の解除
	isStay_ = false;
	//前回停止位置の更新
	preStayPos_ = pos_;
}

void EnemyBase::InitAnim(void)
{
	
}

void EnemyBase::InitUI(void)
{
	//UIコントローラー初期化
	uiCntl_ = std::make_unique<EnemyUIController>(uiPos_,state_, hp_, maxHp_);
	uiCntl_->Init(speciesName_);
}

void EnemyBase::DamageReaction(void)
{
	//ノーリアクション
}

void EnemyBase::UpdateNomal(void)
{
	//移動処理
	(this->*move_)(pPos_);

	//テキトーな移動制限
	if (Utility::MagnitudeF(pos_) > MOVE_MAX) {
		SetPrevPos();
		return;
	}

	//カウンタ増加
	searchRestartCnt_++;

	//サーチ完了時間になったら
	if (searchRestartCnt_ > SEARCH_RESTART_TIME) {
		//判定
		auto deg = Utility::AngleDeg(GetForward(), VSub(pPos_, pos_));

		//索敵可能範囲内に入ったら
		if (Utility::MagnitudeF(VSub(pPos_, pos_)) <= ALERT_DISTANCE &&
			deg <= FIELD_VISION_DEG_HALF) {
			ChangeState(ENEMY_STATE::SEARCH);	//索敵状態に
		}
	}
	
}

void EnemyBase::UpdateSearch(void)
{
	//移動処理
	(this->*move_)(pPos_);
	
	//判定
	auto deg = Utility::AngleDeg(GetForward(), VSub(pPos_, pos_));
	auto distance = Utility::MagnitudeF(VSub(pPos_, pos_));

	//視界内なら
	if (deg <= FIELD_VISION_DEG_HALF &&
		distance <= ALERT_DISTANCE) {
		//一定時間いたら
		if (searchCnt_ >= SEARCH_CNT_MAX) {
			//戦闘状態に
			SoundManager::GetInstance().Play("FindPlayer");
			ChangeState(ENEMY_STATE::BATTLE);
		}
		else {
			searchCnt_++;	//カウンタ増加
		}
	}
	//プレイヤーが索敵範囲外に出たら
	if (distance > ALERT_DISTANCE) {
		ChangeState(ENEMY_STATE::NORMAL);	//通常に戻る
	}
}

void EnemyBase::UpdateBattle(void)
{
	//プレイヤーとの距離
	float distance = Utility::MagnitudeF(VSub(pPos_, pos_));

	//移動処理
	if (distance >= ATTACK_DISTANCE) {
		(this->*move_)(pPos_);
	}
	else {
		OderGoalRot(pPos_);	//回転の設定だけは行う
	}
	
	//カウンタ増加(ゲーム更新スピード)
	intervalCnt_+=SceneManager::GetInstance().GetUpdateSpeedRate();	

	//判定
	//プレイヤーが戦闘状態範囲度外にでたら
	if (distance > BATTLE_FINISH_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NORMAL);
	}

	//プレイヤーが攻撃範囲内かつ攻撃可能な間隔を開けているのなら
	if (distance <= ATTACK_DISTANCE && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		const float ChargeAtkEfcScale = 40.0f;
		const float ChargeAtkEfcSpeed = 0.3f;

		auto& efcM = EffectManager::GetInstance();
		efcM.Play(GetSpeciesName(), "Charge", centerPos_, rot_, ChargeAtkEfcScale, ChargeAtkEfcSpeed);	//攻撃前のチャージエフェクト

		//攻撃する
		atkManager_.Attack(speciesName_,"SwingSword");	//攻撃の発生
		animController_->Play("attack", SPEED_ANIM);	//アニメーション
		stopTime_ = atkManager_.GetTotalTime(EnemyManager::ATTACK_NOMAL);	//攻撃のモーション時間を停止時間に
		intervalCnt_ = 0.0f;	//攻撃間隔リセット
	}
}

void EnemyBase::UpdateDeth(void)
{
	scl_ = VSub(scl_, SCALE_DOWN);	//スケールダウン

	//消滅したら
	if (Utility::LessThanVZero(scl_)) {
		ChangeState(ENEMY_STATE::END);	//ENDに
	}
}

void EnemyBase::MoveNomal(const VECTOR& _pPos)
{
	//ステイ状態のとき
	if (isStay_) {
		//待機アニメーション
		animController_->Play("idle", SPEED_ANIM);

		//一定の時間が過ぎていたら
		if (stayCnt_ >= STAY_TIME) {
			//行先の再設定
			SetNextGoalPos();
		}
		else {
			//引き続きステイ
			//カウンタ増加(ゲーム更新スピード)
			stayCnt_+= SceneManager::GetInstance().GetUpdateSpeedRate();
			return;
		}
	}
	
	//移動(前方方向)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSpeed_* SceneManager::GetInstance().GetUpdateSpeedRate()));
	animController_->Play("walk", SPEED_ANIM);

	//判定
	//停止した位置からどれだけ離れているか
	float diffNow2Pre = Utility::MagnitudeF(VSub(pos_, preStayPos_));

	//既定の移動量以上の値の時
	if (diffNow2Pre >=moveOneTime_) {
		isStay_ = true;	//ステイ状態に
		stayCnt_ = 0;	//カウンタリセット
	}

	
}

void EnemyBase::MoveSearch(const VECTOR& _pPos)
{
}

void EnemyBase::MoveBattle(const VECTOR& _pPos)
{
	//停止時間がまだある場合
	if (stopTime_ > 0) {
		stopTime_--;	//カウンタ減少
		return;
	}

	//移動(前方方向)
	pos_=VAdd(pos_, VScale(GetForward(), moveSpeed_* SceneManager::GetInstance().GetUpdateSpeedRate()));
	animController_->Play("dush", SPEED_ANIM);

	//目標の回転設定
	OderGoalRot(_pPos);
}


void EnemyBase::OderGoalRot(const VECTOR _pPos) {
	//回転
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
	//自分から対象へのベクトル
	auto diff = VSub(_pPos, pos_);
	//角度求める
	auto rad = atan2(diff.x, diff.z);

	//方向の設定
	SetGoalRot(static_cast<float>(rad) - cameraRot.y);
}

void EnemyBase::ChangeState(const ENEMY_STATE _state)
{
	state_ = _state;

	//状態ごとの初期化
	switch (state_)
	{
	case ENEMY_STATE::NORMAL:
		//処理設定
		update_ = &EnemyBase::UpdateNomal;
		move_ = &EnemyBase::MoveNomal;

		moveSpeed_ = MOVE_POW;		//移動速度の設定
		uiCntl_->FindReset();		//発見UIのリセット
		searchRestartCnt_ = 0.0f;	//索敵再開カウンタリセット
		isStay_ = true;	//目的地設定のため

		break;

	case ENEMY_STATE::SEARCH:
		//処理設定
		update_ = &EnemyBase::UpdateSearch;
		move_ = &EnemyBase::MoveSearch;

		searchCnt_ = 0.0f;		//索敵カウンタリセット
		moveSpeed_ = MOVE_POW;	//移動速度の設定
		
		animController_->Play("idle", SPEED_ANIM);	//待機アニメーション再生

		break;

	case ENEMY_STATE::BATTLE:
		//処理設定
		update_ = &EnemyBase::UpdateBattle;
		move_ = &EnemyBase::MoveBattle;

		moveSpeed_ = MOVE_POW_FIND;	//移動速度の設定
		isStay_ = false;			//ステイ状態解除

		break;

	case ENEMY_STATE::DETH:
		//処理設定
		update_ = &EnemyBase::UpdateDeth;

		//死亡アニメーション
		animController_->UnAnimLock();	//アニメーションロック解除
		animController_->Play("dethStart", SPEED_ANIM, { "dethSus" });	//死亡開始→死亡待機の順でアニメーション再生

		//コライダー登録解除
		CollisionManager::GetInstance().MarkForDelete(collider_);		//本体
		CollisionManager::GetInstance().MarkForDelete(atkCollider_);	//攻撃
		atkManager_.DeleteAttackCollider(speciesName_);					//攻撃管理の登録解除

		isActiveGravity_ = false;	//重力無効化

		break;

	case ENEMY_STATE::END:
		isAlive_ = false;	//削除可能に

		break;

	default:
		break;
	}
}

void EnemyBase::Draw(void)
{
	renderer_->Draw();	//モデル描画
	DrawUI();			//UI描画
}

void EnemyBase::DrawUI(void)
{
	//発見マーク
	if (state_ == ENEMY_STATE::SEARCH|| state_ == ENEMY_STATE::BATTLE) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::FIND);
	}

	//HPボックス表示
	if (hp_ >= 0) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::HP);
	}

	//ロックオン関係UI
	//自身がロックオン対象だったら
	if (isLockTarget_) {
		uiCntl_->Draw(EnemyUIController::ENEMY_UI::TARGETTING);
	}
}

const bool EnemyBase::IsAlive(void) const
{
	return state_ != ENEMY_STATE::DETH && state_ != ENEMY_STATE::END;
}

const bool EnemyBase::IsEnd(void) const
{
	return state_ == ENEMY_STATE::END;
}

const void EnemyBase::SetAnimSpeedRate(const float _percent)
{
	animController_->ChangeSpeedRate(_percent);
}

void EnemyBase::SetIsLocked(const bool _flag)
{
	uiCntl_->ChangeTargetUI(_flag);
}

void EnemyBase::Damage(const float _pow)
{
	//ダメージリアクション(派生クラス別)
	DamageReaction();

	//攻撃力分減らす
	hp_ -= static_cast<int>(_pow);
	//戦闘状態ではなかったら
	if (state_ != ENEMY_STATE::BATTLE) {
		//戦闘状態に
		SoundManager::GetInstance().Play("FindPlayer");
		ChangeState(ENEMY_STATE::BATTLE);
	}
	//0以下のとき
	if (hp_ <= 0) {
		//死亡処理
		Deth();
	}
}

void EnemyBase::Deth(void)
{
	ChangeState(ENEMY_STATE::DETH);
}

void EnemyBase::Shout(void)
{
	//ボス専用
}

void EnemyBase::DoHitCollider(const std::weak_ptr<Collider>& _col)
{
	const float DmgEfcScl = 25.0f;
	const float DmgEfcSpeed = 2.5f;
	const float SwordEfcScl = 50.0f;
	const float SwordEfcSpeed = 1.5;

	using TAG = Collider::COL_TAG;
	//プレイヤーの攻撃の場合
	if (_col.lock()->IsContainsAllTag({ TAG::PLAYER,TAG::ATTACK })) {
		//ダメージ処理
		Damage(_col.lock()->GetPower());
		auto& efcM = EffectManager::GetInstance();
		efcM.Play(GetSpeciesName(), "Damage", centerPos_, rot_, DmgEfcScl, DmgEfcSpeed, "Damage");	//ダメージエフェクト
		efcM.Play(GetSpeciesName(), "Sword", centerPos_, rot_, SwordEfcScl, SwordEfcSpeed);			//斬撃エフェクト

		//攻撃なのでフラグをオフに
		_col.lock()->SetUseThis(false);
	}

	//通常移動時、壁とぶつかったら
	if (state_ == ENEMY_STATE::NORMAL && _col.lock()->IsContainsTag(TAG::WALL)) {
		pos_ = prevPos_;	//前回位置に戻す
		SetNextGoalPos();	//行先の再設定
	}
}
