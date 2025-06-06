#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../Utility/Utility.h"
#include "EnemyBase.h"

namespace {

	constexpr VECTOR forward={0.0f,0.0f,100.0f};

	//デバッグ用
	constexpr int serchDebugCol = 0x6495ef;
	constexpr int alertDebugCol = 0xff0000;

	constexpr int serchDebugCol2 = 0xffff00;
	constexpr int alertDebugCol2 = 0xffdd88;
}

EnemyBase::EnemyBase(void)
{
	serchCol_ = serchDebugCol;
	alertCol_ = serchDebugCol2;
	color_ = 0xffffff;

	update_ = &EnemyBase::UpdateNomal;
	move_ = &EnemyBase::MoveNomal;

	moveOneTime_ = -1.0f;

	debugRot_ = -1.0;
	preStayPos_ = Utility::VECTOR_INIT;

	//行先設定のため初期はステイ状態にする
	isStay_ = true;
	stayCnt_ = STAY_TIME;
	stopTime_ = -1.0f; 
	intervalCnt_ = INTERVAL_ATTACK_NOMAL;

	hp_ = 5;
	moveSped_ = MOVE_POW;

	isDelete_ = false;
}

EnemyBase::~EnemyBase(void)
{
}

const bool EnemyBase::Init(void)
{

	SetPram();
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);
	if (modelId_ == -1) {
		return false;
	}
	//パラメータ関係
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	pos_ = { 0.0f,0.0f,1000.0f };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//初期化用に一回実行
	UpdateRotQuat();
	//状態を通常に
	ChangeState(ENEMY_STATE::NOMAL);
	//アニメーション初期化
	AnimInit();
	animController_->Play("idle", SPEED_ANIM);

	return true;
}


void EnemyBase::Update(const VECTOR _pPos, AttackManager& _atk)
{
	(this->*update_)(_pPos,_atk);
	//共通更新
	Rotation();
	UpdateRotQuat();

	animController_->Update();
}

void EnemyBase::SetPram(void)
{
	//各敵たち
	//後々Jsonやったら楽になるかも？
}

void EnemyBase::AnimInit(void)
{
	animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("attack", ANIM_ATTACK_NOMAL, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dush", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}




void EnemyBase::UpdateNomal(const VECTOR& _pPos, AttackManager& _atk)
{
	//移動処理
	(this->*move_)(_pPos);

	//判定
	//索敵可能範囲内に入ったら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) <= ALERT_DISTANCE) {
		//索敵状態に
		ChangeState(ENEMY_STATE::SEARCH);
	}
}

void EnemyBase::UpdateSearch(const VECTOR& _pPos, AttackManager& _atk)
{
	//移動処理
	(this->*move_)(_pPos);

	//判定
	auto deg = Utility::AngleDeg(GetForward(), VSub(_pPos, pos_));
	auto distance = Utility::MagnitudeF(VSub(_pPos, pos_));

	//視界内なら
	if (deg <= FIELD_VISION_DEG_HALF && 
		distance <= FIELD_VISION_DISTANCE) {
		//戦闘状態に
		ChangeState(ENEMY_STATE::BATTLE);
	}
	//プレイヤーが索敵範囲外に出たら
	else if (distance > ALERT_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}

	debugRot_ = deg;
}

void EnemyBase::UpdateBattle(const VECTOR& _pPos, AttackManager& _atk)
{
	//この内容は初期キャラ用。攻撃時には止まって攻撃する
	//強いキャラクターは移動攻撃も想定するのでここの処理とは少し違っていくる
	
	//移動処理
	(this->*move_)(_pPos);

	intervalCnt_++;

	//判定
	//プレイヤーが索敵範囲外にでたら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) > ALERT_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}

	//プレイヤーが攻撃範囲内かつ攻撃可能な間隔を開けているのなら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) <= ATTACK_DISTANCE && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		//攻撃する
		_atk.Attack(EnemyManager::ATTACK_NOMAL, POW_ATTACK_NOMAL, VAdd(pos_, characterRotY_.PosAxis(RELATIVE_ATTACK_POS)), characterRotY_, AttackManager::ATTACK_MASTER::ENEMY, SCALE_ATTACK_NOMAL);
		animController_->Play("attack", SPEED_ANIM);
		stopTime_ = _atk.GetTotalTime(EnemyManager::ATTACK_NOMAL);
		intervalCnt_ = 0.0f;
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
			//行先の角度設定(characterRotYに変更すべし)
			VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
			float degRand = static_cast<float>(GetRand(static_cast<int>(Utility::CIRCLE_DEG)));
			float radRand = Utility::Deg2RadF(degRand) - cameraRot.y;
			//クォータニオンに変換
			Quaternion quaGoal= Quaternion::AngleAxis((double)cameraRot.y + radRand, Utility::AXIS_Y);
			//キャラクター回転も設定
			SetGoalRot(radRand);
			characterRotY_ = quaGoal;

			//移動量を範囲付きのランダムで生成
			moveOneTime_ = static_cast<float>(GetRand(MOVE_RANDOM_MAX)) + MOVE_RANDOM_MIN;
			//ステイ状態の解除
			isStay_ = false;
			//前回停止位置の更新
			preStayPos_ = pos_;
		}
		else {
			//引き続きステイ
			stayCnt_++;
			return;
		}
	}
	
	//移動(前方方向)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSped_));
	animController_->Play("walk", SPEED_ANIM);

	//判定
	//停止した位置からどれだけ離れているか
	float diffNow2Pre = Utility::MagnitudeF(VSub(pos_, preStayPos_));

	//既定の移動量以上の値の時
	if (diffNow2Pre >=moveOneTime_) {
		//ステイ状態に
		isStay_ = true;
		stayCnt_ = 0;
	}

	
}

void EnemyBase::MoveSearch(const VECTOR& _pPos)
{
	//実装予定
	//＜敵のひきつけ＞
	//ほかのオブジェクトの使用による効果なので初期では作らない
}

void EnemyBase::MoveBattle(const VECTOR& _pPos)
{
	//停止時間がまだある場合
	if (stopTime_ > 0) {

		stopTime_--;
		return;
	}
	//移動(前方方向)
	pos_=VAdd(pos_, VScale(GetForward(), moveSped_));
	animController_->Play("dush", SPEED_ANIM);

	//回転
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
	//自分から対象へのベクトル
	auto diff = VSub(_pPos, pos_);
	//角度求める
	auto rad = atan2(diff.x, diff.z);

	//方向の設定
	SetGoalRot(static_cast<float>(rad) - cameraRot.y);

	debugRot_ = Utility::Deg2RadF(rad);
}

void EnemyBase::ChangeState(const ENEMY_STATE _state)
{
	switch (_state)
	{
	case ENEMY_STATE::NOMAL:
		update_ = &EnemyBase::UpdateNomal;
		move_ = &EnemyBase::MoveNomal;
		moveSped_ = MOVE_POW;

		serchCol_ = serchDebugCol;
		alertCol_ = serchDebugCol2;
		break;

	case ENEMY_STATE::SEARCH:
		update_ = &EnemyBase::UpdateSearch;
		move_ = &EnemyBase::MoveSearch;
		moveSped_ = MOVE_POW;

		serchCol_ = serchDebugCol;
		alertCol_ = alertDebugCol2;
		break;

	case ENEMY_STATE::BATTLE:
		update_ = &EnemyBase::UpdateBattle;
		move_ = &EnemyBase::MoveBattle;
		moveSped_ = MOVE_POW_FIND;

		serchCol_ = alertDebugCol;
		break;
	case ENEMY_STATE::DETH:
		break;
	case ENEMY_STATE::END:
		//削除可能に
		isDelete_ = true;
		break;
	default:
		break;
	}
}

void EnemyBase::SetColor(int _color)
{
	color_ = _color;
}

void EnemyBase::SetPos(VECTOR _pos)
{
	pos_ = _pos;
}

void EnemyBase::DrawDebug(void)
{
	VECTOR forwardDif = { 0,0,100 };
	//前方向明示
	VECTOR fowardPos = VAdd(pos_, characterRotY_.PosAxis(forwardDif));
	DrawSphere3D(fowardPos, 8, 8, 0xff5500, 0xff0000, false);
	//視野角
	auto conePos = VAdd(pos_, VECTOR{ 0.0f,-1.0f,0.0f });

	DrawCone3D(conePos, pos_, ALERT_DISTANCE, 30, alertCol_, 0x000000, true);

	DrawCone3D(conePos, pos_, FIELD_VISION_DISTANCE, 30, serchCol_, 0x000000, true);

	/*VECTOR fowardDir = VAdd(pos_, characterRotY_.PosAxis(forward));
	DrawFormatString(0, 0, 0xffffff, "EPOS={%.1f,%.1f,%.1f}\nDEG={%.1f}\nForward={%.1f,%.1f,%.1f}",
		pos_.x, pos_.y, pos_.z,
		static_cast<float>(debugRot_),
		fowardDir.x, fowardDir.y, fowardDir.z);*/

	DrawCupcel();
}

const bool EnemyBase::IsAlive(void) const
{
	return isDelete_;
}
