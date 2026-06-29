#include"../Application.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Decoration/UIManager2d.h"
#include"../Object/Stage/StageManager.h"
#include"../Object/Character/Player/PlayerChara.h"
#include"../Utility/Utility.h"
#include"Ability/MagnetCatch.h"
#include"Ability/LockTime.h"
#include "AbilityManager.h"

const std::string AbilityManager::UI_ABILITY_MGNET = "MagnetIcon";
const std::string AbilityManager::UI_ABILITY_LOCK_TIME = "LockTimeIcon";
const VECTOR AbilityManager::ABILITY_ICON_POS = { 170.0f,230.0f,0.0f };
const VECTOR AbilityManager::RETICLE_POS = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 ,0.8f };	//レティクル位置(スクリーン座標)

//ローカル定数
namespace {
	const FLOAT4 NONE_COLOR = { 0.0f,0.0f,0.0f,1.0f };				//物体通常色
	const FLOAT4 MAGNET_COLOR = { 1.0f,0.0f,0.0f,1.0f };			//能力色(マグネット)
	const FLOAT4 LOCK_TIME_COLOR = { 1.0f,1.0f,0.0f,1.0f };			//能力色(ロックタイム)
	const FLOAT4 SELECT_COLOR_MAGNET = { 1.0,1.0f,0.0f ,1.0f};		//選択色(マグネット)
	const FLOAT4 SELECT_COLOR_LOCK_TIME = { 0.0,1.0f,0.0f ,1.0f};	//選択色(ロックタイム)

	const int RETICLE_COLOR = 0x55ff00;			//レティクル通常色
	const float HIT_RETICLE_DIFF = 80.0f;		//レティクル当たり判定大きさ
	const float RETICLE_SIZE = 10.0f;			//レティクル大きさ

	const std::string UI_RETICLE = "ReticleIcon";	//レティクルUI名
	const float RETICLE_UI_EX = 1.5f;				//レティクルUI拡大率
}

AbilityManager::AbilityManager(StageManager& _stage, PlayerChara& _master)
	: master_(_master)
	, stage_(_stage)
	, useAbility_(ABILITY_TYPE::MAGNET)
	, state_(STATE::END)
	, isPlayerAnyInput_(true)
	,update_(&AbilityManager::UpdateEnd)
{
	iconNames_[static_cast<int>(ABILITY_TYPE::MAGNET)] = UI_ABILITY_MGNET;
	iconNames_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = UI_ABILITY_LOCK_TIME;

	abilities_[static_cast<int>(ABILITY_TYPE::MAGNET)] = std::make_unique<MagnetCatch>(*this, master_);
	abilities_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = std::make_unique<LockTime>(*this);

	selectColores_[static_cast<int>(ABILITY_TYPE::MAGNET)] = SELECT_COLOR_MAGNET;
	selectColores_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = SELECT_COLOR_LOCK_TIME;

	auto& resM = ResourceManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	//マグネットアイコン
	uiM.Add(UI_ABILITY_MGNET, resM.Load(ResourceManager::SRC::ABILITY_MAGNET_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_MGNET, ABILITY_ICON_POS, ABILITY_UI_EX);

	//タイムロックアイコン
	uiM.Add(UI_ABILITY_LOCK_TIME, resM.Load(ResourceManager::SRC::ABILITY_LOCK_TIME_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_LOCK_TIME, ABILITY_ICON_POS, ABILITY_UI_EX);

	//レティクル
	uiM.Add(UI_RETICLE, resM.Load(ResourceManager::SRC::RETICLE_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_RETICLE, RETICLE_POS, RETICLE_UI_EX);
}

AbilityManager::~AbilityManager(void)
{
}

void AbilityManager::Update(void)
{
	(this->*update_)();
}

void AbilityManager::Draw(void)
{
	//アイコンの描画
	if (useAbility_ != ABILITY_TYPE::MAX && useAbility_ != ABILITY_TYPE::MAX) {
		UIManager2d::GetInstance().Draw(iconNames_[static_cast<int>(useAbility_)]);
	}

	//能力使用時のレティクル
	if (state_ == STATE::REDY || state_ == STATE::DIRECTION) {
		UIManager2d::GetInstance().Draw(UI_RETICLE);
	}

	//アビリティごとの固有描画
	abilities_[static_cast<int>(useAbility_)]->Draw();
}

void AbilityManager::ChangeAbility(const ABILITY_TYPE _type)
{
	useAbility_ = _type;
}

FLOAT4 AbilityManager::GetAbilityColor(const ABILITY_TYPE _type)
{
	FLOAT4 ret;

	if (_type == ABILITY_TYPE::MAGNET) {
		ret = MAGNET_COLOR;
	}
	else {
		ret = LOCK_TIME_COLOR;
	}

	return ret;
}

bool AbilityManager::IsHitReticle(const VECTOR& _screenPos)
{
	float diff = fabs(Utility::MagnitudeF(VSub(_screenPos, RETICLE_POS)));
	bool ret = false;
	if (diff <= HIT_RETICLE_DIFF) {
		ret = true;
	}
	return ret;
}

bool AbilityManager::IsNearObject2Camera(const VECTOR& _pos1, const VECTOR& _pos2)
{
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	float diff1 = Utility::MagnitudeF(VSub(_pos1, cameraPos));
	float diff2 = Utility::MagnitudeF(VSub(_pos2, cameraPos));
	return diff1 <= diff2;
}

void AbilityManager::UpdateRedy(void)
{
	//レティクルとの当たり判定
	std::weak_ptr<GimmickObjBase> hitReticleObj;
	for (auto& obj : stage_.GetAffectAbilityObjectes()) {
		//衝突していたら
		if (IsHitReticle(obj.lock()->GetScreenPos())) {
			//まだ参照するものがない場合
			if (hitReticleObj.expired()) {
				hitReticleObj = obj;
			}
			else {
				//既に何かしら入っている場合
				//新しいオブジェクトの方が近い時
				if (!IsNearObject2Camera(hitReticleObj.lock()->GetPos(), obj.lock()->GetPos())) {
					//近いほうを採用
					hitReticleObj = obj;
				}
			}
		}
	}

	//対象となるものがなかった時
	if (hitReticleObj.expired()) {
		//現在選択されているものの解除
		if (!selectObj_.expired()) {
			//通常色の設定
			selectObj_.lock()->SetObjectRenderColor(GetAbilityColor(useAbility_));
			//選択解除
			selectObj_.reset();
		}
	}
	else {
		//新しく選択されたものの設定
		if (selectObj_.lock() != hitReticleObj.lock()) {
			selectObj_ = hitReticleObj;
			selectObj_.lock()->SetObjectRenderColor(selectColores_[static_cast<int>(useAbility_)]);
		}
	}
}

void AbilityManager::UpdateDirection(void)
{
	abilities_[static_cast<int>(useAbility_)]->UpdateDirection(selectObj_);
}

void AbilityManager::UpdateUse(void)
{
	abilities_[static_cast<int>(useAbility_)]->UpdateUse(selectObj_);
}

void AbilityManager::UpdateEnd(void)
{
	//何もしない
}

const VECTOR AbilityManager::GetFollowPosForUseMagnet(const VECTOR _playerPos)
{
	VECTOR retPos=_playerPos;
	const float halfRate = 0.5f;	//半分の定義

	//プレイヤーと能力対象の距離
	VECTOR distance = VSub(selectObj_.lock()->GetPos(), _playerPos);	
	//距離の中央を取得
	distance = VScale(distance, halfRate);	

	//プレイヤーからの差分として合成
	retPos = VAdd(retPos, distance);	

	return retPos;
}

void AbilityManager::ChangeState(const STATE _next)
{
	//状態の変更
	state_ = _next;

	//状態開始に応じた処理
	switch (state_)
	{
	case AbilityManager::STATE::REDY:
		//アビリティ準備
		RedyAbility();
		break;
	case AbilityManager::STATE::DIRECTION:
		//演出
		DirectionAbility();
		break;
	case AbilityManager::STATE::USE:
		//使用開始
		UseAbility();
		break;
	case AbilityManager::STATE::END:
		//終了
		EndUsingAbility();
		break;
	default:
		break;
	}
}


void AbilityManager::RedyAbility(void)
{
	//色の設定
	stage_.SetAbilityColor(GetAbilityColor(useAbility_));
	//更新処理の設定
	update_ = &AbilityManager::UpdateRedy;
}

void AbilityManager::DirectionAbility(void)
{
	//能力の状況リセット
	abilities_[static_cast<int>(useAbility_)]->ResetAbility();
	//更新処理の設定
	update_ = &AbilityManager::UpdateDirection;
}

void AbilityManager::UseAbility(void)
{
	//全体の付与色をなくす
	stage_.SetAbilityColor(NONE_COLOR);
	//対象のオブジェクトは能力色を付与
	selectObj_.lock()->SetObjectRenderColor(GetAbilityColor(useAbility_));
	selectObj_.lock()->ResetGravity();
	selectObj_.lock()->SetIsAffecting(true);

	//マグネット時はプレイヤーとの当たり判定を除外
	if(useAbility_==ABILITY_TYPE::MAGNET){
		selectObj_.lock()->GetCollider().lock()->AddNoHitTag(Collider::COL_TAG::PLAYER);
		selectObj_.lock()->GetCollider().lock()->AddNoHitTag(Collider::COL_TAG::FALL_LINE);
	}

	//更新処理の設定
	update_ = &AbilityManager::UpdateUse;

	//マグネットの時はプレイヤーの入力を無効化
	if(useAbility_==ABILITY_TYPE::MAGNET){
		isPlayerAnyInput_ = false;
	}
}

void AbilityManager::EndUsingAbility(void)
{
	//使用終了
	if (!selectObj_.expired()) {
		//マグネット時はプレイヤーとの当たり判定を戻す
		if (useAbility_ == ABILITY_TYPE::MAGNET) {
			selectObj_.lock()->GetCollider().lock()->DeleteNoHitTag(Collider::COL_TAG::PLAYER);
			selectObj_.lock()->GetCollider().lock()->DeleteNoHitTag(Collider::COL_TAG::FALL_LINE);
		}
		//終了処理および監視からの解放
		selectObj_.lock()->FinishAffect();
		selectObj_.lock()->SetIsAffecting(false);
		selectObj_.reset();
	}
	
	//アビリティ固有の終了処理
	abilities_[static_cast<int>(useAbility_)]->EndAbility();

	//付与色をなくす
	stage_.SetAbilityColor(NONE_COLOR);

	//更新処理の設定
	update_ = &AbilityManager::UpdateEnd;

	//カメラを通常の追従に戻す
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
	//プレイヤーの入力を有効化
	isPlayerAnyInput_ = true;
}