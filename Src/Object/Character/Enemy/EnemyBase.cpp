#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
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

	goalPos_ = Utility::VECTOR_INIT;

	debugRot_ = -1.0;
}

EnemyBase::~EnemyBase(void)
{
}

const bool EnemyBase::Init(void)
{

	SetPram();
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_MDL).handleId_;
	if (modelId_ == -1) {
		return false;
	}
	//パラメータ関係
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	pos_ = { 0.0f,0.0f,1000.0f };
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	//初期化用に一回実行
	UpdateRotQuat();
	//状態を通常に
	ChangeState(ENEMY_STATE::NOMAL);
	return true;
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

	VECTOR fowardDir = VAdd(pos_, characterRotY_.PosAxis(forward));
	DrawFormatString(0, 0, 0xffffff, "EPOS={%.1f,%.1f,%.1f}\nDEG={%.1f}\nForward={%.1f,%.1f,%.1f}",
		pos_.x, pos_.y, pos_.z,
		static_cast<float>(debugRot_),
		fowardDir.x, fowardDir.y, fowardDir.z);
}

void EnemyBase::Update(const VECTOR _pPos)
{
	(this->*update_)(_pPos);
	//共通更新
	Rotation();
	UpdateRotQuat();
}

void EnemyBase::SetPram(void)
{
	//各敵たち
	//後々Jsonやったら楽になるかも？
}

void EnemyBase::UpdateNomal(const VECTOR& _pPos)
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

void EnemyBase::UpdateSearch(const VECTOR& _pPos)
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

void EnemyBase::UpdateBattle(const VECTOR& _pPos)
{
	//移動処理
	(this->*move_)(_pPos);

	//判定
	//プレイヤーが索敵範囲外にでたら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) > ALERT_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}
}

void EnemyBase::MoveNomal(const VECTOR& _pPos)
{

	//現在地から目標値へのベクトル
	VECTOR diff = VSub(goalPos_, pos_);

	//目標値より行き過ぎていたら
	if (Utility::MagnitudeF(diff) <= 0.0f) {
		//行先の再設定
		//行先の角度設定(characterRotYに変更すべし)
		Quaternion quaRand;
		//移動量乱数
		float moveRand = static_cast<float>(GetRand(MOVE_RANDOM_MAX)) + MOVE_RANDOM_MIN;
		//前方方向に移動するベクトルに変換
		VECTOR moveDir = Utility::VECTOR_ZERO;
		moveDir.z = moveRand;
		//行先設定
		goalPos_ = VAdd(pos_, quaRand.PosAxis(moveDir));
	}
}

void EnemyBase::MoveSearch(const VECTOR& _pPos)
{
}

void EnemyBase::MoveBattle(const VECTOR& _pPos)
{
	//移動(前方方向)
	pos_=VAdd(pos_, VScale(GetForward(), MOVE_POW));

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

		serchCol_ = serchDebugCol;
		alertCol_ = serchDebugCol2;
		break;

	case ENEMY_STATE::SEARCH:
		update_ = &EnemyBase::UpdateSearch;
		move_ = &EnemyBase::MoveSearch;

		serchCol_ = serchDebugCol;
		alertCol_ = alertDebugCol2;
		break;

	case ENEMY_STATE::BATTLE:
		update_ = &EnemyBase::UpdateBattle;
		move_ = &EnemyBase::MoveBattle;

		serchCol_ = alertDebugCol;
		break;
	default:
		break;
	}
}

