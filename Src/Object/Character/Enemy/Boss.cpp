#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Utility/Utility.h"
#include"../../../UI/EnemyUIController.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../Common/Collider.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/Geometry/Capsule.h"
#include "Boss.h"

//ローカル定数
namespace {
#pragma region アニメーション関連
	const int BOSS_IDLE = 1;
	const int BOSS_WALK = 2;
	const int BOSS_PRE_PUNCH = 5;
	const int BOSS_PUNCH = 6;
	const int BOSS_PRE_SHOUT = 9;
	const int BOSS_SHOUT = 10;
	const int BOSS_DETH = 12;
#pragma endregion

	const float BOSS_HP = 300.0f;
	const float BOSS_RADIUS = 400.0f;

	const float BOSS_ATTACK_SCALE = 500.0f;

	const float BOSS_ATTACK_RELATIVE_Y = 75.0f;
}

Boss::Boss(VECTOR& _pos, const int _num, AttackManager& _atk, const VECTOR& _pPos)
	:EnemyBase(_pos,_num,_atk,_pPos)
{
}

Boss::~Boss(void)
{
}

void Boss::Shout(void)
{
	animController_->Play("preShout", 0.8f, { "shout" });
	SoundManager::GetInstance().Play("shout");
}

void Boss::InitAnim(void)
{
	animController_->Add("idle", BOSS_IDLE, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("preAttack", BOSS_PRE_PUNCH, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("attack", BOSS_PUNCH, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("preShout", BOSS_PRE_SHOUT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("shout", BOSS_SHOUT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("walk", BOSS_WALK, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dethStart", BOSS_DETH, AnimationController::PLAY_TYPE::NOMAL);
	

	//その場しのぎでここに鳴き声入れる
	SoundManager::GetInstance().Add(SoundManager::TYPE::SE, "shout", ResourceManager::GetInstance().Load(ResourceManager::SRC::BOSS_SHOUT_SE).handleId_);
}

void Boss::SetParam(void)
{
	speciesName_ = "Boss";

	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::BOSS_MDL);

	if (modelId_ == -1) {
		return;
	}
	//パラメータ関係
	const float SCALE = 0.02f;
	scl_ = { SCALE,SCALE ,SCALE };
	preStayPos_ = pos_;
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);

	//ボスはもしかしたらモデルで判定するかも
	//当たり判定大きさ
	colRadius_ = BOSS_RADIUS;
	//当たり判定生成
	headPos_ = VAdd(pos_, CHARACTER_HEIGHT);	//頭位置
	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::ENEMY},
		std::move(std::make_unique<Capsule>(headPos_, pos_, colRadius_))
	, std::set<TAG>{TAG::STAGE, TAG::OBJECT});

	//攻撃可能距離
	atkDistance_ = BOSS_ATTACK_SCALE;

	//攻撃の発生位置(相対座標)
	atkRelative_ = VECTOR{ 0.0f,BOSS_ATTACK_RELATIVE_Y,atkDistance_ };


	//攻撃
	atkPos_ = VAdd(pos_, atkRelative_);
	atkCollider_ = std::make_shared<Collider>(*this, std::set<TAG>{ TAG::ENEMY, TAG::ATTACK },
		std::move(std::make_unique<Sphere>(atkPos_, BOSS_ATTACK_SCALE))
	, std::set<TAG>{TAG::STAGE, TAG::OBJECT});

	atkCollider_->SetUseThis(false);

	CollisionManager::GetInstance().AddCollider(atkCollider_);	//当たり判定登録

	//アニメーション初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	animController_->Play("idle", SPEED_ANIM);

	hp_ = BOSS_HP;
	maxHp_ = BOSS_HP;

	uiDeviationY_ = 600.0f;

	//位置設定
	uiPos_ = pos_;
	//頭位置
	uiPos_.y += uiDeviationY_;

	//UI初期化
	InitUI();

	//モデル描画クラス生成
	material_ = std::make_unique<ModelMaterial>("BlurSkinVS.cso", 2, "BlurSkinPS.cso", 3);
	//VS

	intervalCnt_ = 0.0f;
	//PS
	//各色の強さ(拡散光)
	material_->AddConstBufPS({ 1.0f,1.0f,1.0f,1.0f });
	//ブラーの強さ(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });
	//サンプル数(最初の項目のみ関係する)
	material_->AddConstBufPS({ 1.0f,0.0f,0.0f,0.0f });

	//状態を通常に
	ChangeState(ENEMY_STATE::BATTLE);
}

void Boss::UpdateBattle(void)
{
	//この内容は初期キャラ用。攻撃時には止まって攻撃する
	//強いキャラクターは移動攻撃も想定するのでここの処理とは少し違っていくる
	//プレイヤーとの距離
	float distance = Utility::MagnitudeF(VSub(pPos_, pos_));

	//移動処理
	//移動処理
	if (distance >= atkDistance_) {
		(this->*move_)(pPos_);
	}
	else {
		OderGoalRot(pPos_);	//回転の設定だけは行う
	}


	//カウンタ増加(ゲーム更新スピード)
	intervalCnt_ += SceneManager::GetInstance().GetUpdateSpeedRate_();

	//判定
	//プレイヤーが攻撃範囲内かつ攻撃可能な間隔を開けているのなら
	if (distance <= atkDistance_ && intervalCnt_ > INTERVAL_ATTACK_NOMAL) {
		//攻撃の準備時間

		//準備時間が終わったら攻撃する
		atkManager_.Attack(speciesName_,"SwingSword");
		animController_->Play("attack", SPEED_ANIM);
		stopTime_ = atkManager_.GetTotalTime(EnemyManager::ATTACK_NOMAL);
		intervalCnt_ = 0.0f;
	}
}

void Boss::MoveBattle(const VECTOR& _pPos)
{
	//停止時間がまだある場合
	if (stopTime_ > 0) {
		stopTime_--;
		return;
	}
	//移動(前方方向)
	pos_ = VAdd(pos_, VScale(GetForward(), moveSped_ * SceneManager::GetInstance().GetUpdateSpeedRate_()));
	animController_->Play("walk", SPEED_ANIM);

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

void Boss::DrawUI(void)
{
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

