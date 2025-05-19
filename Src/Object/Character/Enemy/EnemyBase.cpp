#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Utility/Utility.h"
#include "EnemyBase.h"

namespace {
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
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };

	pos_ = { 0.0f,0.0f,1000.0f };
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);

	UpdateRotQuat();
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
	//索敵可能範囲内に入ったら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) <= ALERT_DISTANCE) {
		//索敵状態に
		ChangeState(ENEMY_STATE::SEARCH);
	}

	//通常の移動処理

}

void EnemyBase::UpdateSearch(const VECTOR& _pPos)
{
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
}

void EnemyBase::UpdateBattle(const VECTOR& _pPos)
{
	auto deg = Utility::AngleDeg(pos_, VSub(_pPos, pos_));
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度
	//敵より右側にいたら
	if (pos_.x > _pPos.x) {
		//AngleDegでは0~180なので調整する
		deg = Utility::CIRCLE_HALF_DEG + (Utility::CIRCLE_HALF_DEG - deg);
	}
	//方向の設定
	SetGoalRot(Utility::Deg2RadF(static_cast<float>(deg)) - cameraRot.y);

	//プレイヤーが索敵範囲外にでたら
	if (Utility::MagnitudeF(VSub(_pPos, pos_)) > ALERT_DISTANCE) {
		//通常に戻る
		ChangeState(ENEMY_STATE::NOMAL);
	}
}

void EnemyBase::ChangeState(const ENEMY_STATE _state)
{
	switch (_state)
	{
	case ENEMY_STATE::NOMAL:
		update_ = &EnemyBase::UpdateNomal;

		serchCol_ = serchDebugCol;
		alertCol_ = serchDebugCol2;
		break;

	case ENEMY_STATE::SEARCH:
		update_ = &EnemyBase::UpdateSearch;

		serchCol_ = serchDebugCol;
		alertCol_ = alertDebugCol2;
		break;

	case ENEMY_STATE::BATTLE:
		update_ = &EnemyBase::UpdateBattle;

		serchCol_ = alertDebugCol;
		break;
	default:
		break;
	}
}

