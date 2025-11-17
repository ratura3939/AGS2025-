#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/GameSystem/AnimationController.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../../../Utility/Utility.h"
#include"../../Common/Collider.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include "PlayerChara.h"

//ローカル定数
namespace {
#pragma region ステータス定数
	const int PALYER_HP = 5;		//HP
	const float MOVE_POW = 5.0f;	//移動量
	const float DUSH_POW = 20.0f;	//移動量(ダッシュ)
	const float CHARA_SCALE = 0.7f;	//サイズ
	const VECTOR FOCUS_NOMAL = { 0.0f,0.0f,50.0f };      //注視点(通常)

	const int ALLERT_TIME = 30;  //警戒UI描画時間   
	const std::string UI_NAME = "PlayerUI"; //UIリソース名
	const std::string EFC_NAME = "PlayerEfc"; //UIリソース名
	const std::string CHARACTER_NAME = "Player"; //UIリソース名
#pragma endregion

#pragma region アニメーション関連定数
	const float SPEED_ANIM = 1.0f;
	const int ANIM_IDLE = 36;            //待機
	//攻撃
	const int ANIM_ATTACK_FIRST = 8;     //攻撃一段目
	const int ANIM_ATTACK_SECOND = 9;    //攻撃二段目
	const int ANIM_ATTACK_SPIN = 10;     //回転切り
	//防御
	const int ANIM_GUARD_START = 18;     //防御開始
	const int ANIM_GUARD_ATTACK = 19;    //ガードアタック
	const int ANIM_GUARD_HIT = 20;       //ガードヒット
	const int ANIM_GUARD_SUSTANABLE = 21;//ガード持続

	//その他アクション
	const int ANIM_WALK = 72;            //歩き
	const int ANIM_DUSH_FORWARD = 48;    //ダッシュ(前方)
	const int ANIM_DUSH_LEFT = 50;       //ダッシュ(左)<ロックオン時のみ>
	const int ANIM_DUSH_RIGHT = 51;      //ダッシュ(右)<ロックオン時のみ>

	const int ANIM_JUMP = 39;            //ジャンプ
	const int ANIM_DODGE_LEFT = 29;      //回避(左)
	const int ANIM_DODGE_RIGHT = 30;     //回避(右)
	const int ANIM_DODGE_BACK = 27;      //回避(後)

	const int ANIM_PICK_UP = 47;         //拾う

	//演出
	const int ANIM_DAMAGE = 35;          //被ダメ
	const int ANIM_DETH_START = 25;      //死亡開始
	const int ANIM_DETH_SUSTANABLE = 26; //死亡持続
#pragma endregion

	//攻撃関連
	float ATK_SCALE = 70.0f;
	float ATK_POWER = 30.0f;
	VECTOR ATK_LOCAL_POS = { 0.0f, 75.0f, 100.0f };	//攻撃相対座標

	const float GRAVITY_POW = 1.0f; //重力
	const float JUMP_POW = 80.0f; //ジャンプ力
}


PlayerChara::PlayerChara(AttackManager& _atk)
	: atkMng_(_atk)
	, isSlow_(false)
{
	focusPoint_ = Utility::VECTOR_ZERO;
	lockState_ = LOCK_STATE::MAX;
	state_ = STATE::NOMAL;
	isDush_ = false;
	afterMoveRad_ = 0.0f;
	speciesName_ = CHARACTER_NAME;
	jumpPow_ = 0.0f;
}

PlayerChara::~PlayerChara(void)
{
}

void PlayerChara::DoInit(void)
{
	//モデル基礎情報
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_MDL).handleId_;

	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT),0.0f);

	lockState_ = LOCK_STATE::NOMAL;

	//当たり判定
	headPos_ = VAdd(pos_, CHARACTER_HEIGHT);	//頭位置
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::PLAYER},
		std::move(std::make_unique<Capsule>(headPos_, pos_, CHARACTER_RADIUS)));

	//攻撃用当たり判定
	power_ = ATK_POWER;
	atkPos_ = VAdd(pos_, ATK_LOCAL_POS);
	atkCollider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{ COL_TYPE::PLAYER,COL_TYPE::ATTACK },
		std::move(std::make_unique<Sphere>(atkPos_, ATK_SCALE)));


	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	atkCollider_->SetUseThis(false);

	CollisionManager::GetInstance().AddCollider(atkCollider_);	//当たり判定登録

	//注視点の設定
	focusPoint_ = FOCUS_NOMAL;

	hp_ = PALYER_HP;

	//アニメ初期化
	animController_ = std::make_unique<AnimationController>(modelId_);
	InitAnim();
	//初期アニメ
	animController_->Play("idle", SPEED_ANIM);

	//UI初期化
	InitUI();
}

void PlayerChara::DoUpdate(void)
{
	headPos_ = VAdd(pos_, CHARACTER_HEIGHT);	//頭位置
	atkPos_ = VAdd(pos_, characterRotY_.PosAxis(ATK_LOCAL_POS));
	uiPos_ = pos_;
	uiPos_.y += 200.0f;
	allertTime_++;
	//ほかにアクション行動していないときのみ
	if (state_ == STATE::NOMAL || lockState_ == LOCK_STATE::LOCKON) {
		Move();

		//ロックオンのとき
		if (lockState_ == LOCK_STATE::LOCKON) {
			//敵との角度差を設定
			afterMoveRad_ = GetToLockDeg();
		}

		//目標角度設定
		SetGoalRot(afterMoveRad_);

		Rotation();
		if (allertTime_ > ALLERT_TIME) {
			uiCntl_->ChangeAllert(false);
		}
	}

	//ジャンプ
	jumpPow_ += GRAVITY_POW;
	if (jumpPow_ <= 0.0f) {
		jumpPow_ = 0.0f;
	}
	pos_.y += jumpPow_;

	animController_->Update();
	uiCntl_->Update();
}

const VECTOR PlayerChara::GetFocusPoint(void) const
{
	return VAdd(pos_, focusPoint_);
}

void PlayerChara::ChangeLockState(const bool _state)
{
	if (_state) {
		lockState_ = LOCK_STATE::LOCKON;
		//プレイヤーの角度を強制的に敵に向ける
		float deg = GetToLockDeg();
		SetGoalRot(deg);
		characterRotY_ = goalQua_;
	}
	else lockState_ = LOCK_STATE::NOMAL;
}

const PlayerChara::STATE PlayerChara::GetState(void) const
{
	return state_;
}

const PlayerChara::LOCK_STATE& PlayerChara::GetLockState(void) const
{
	return lockState_;
}

void PlayerChara::SetState(const STATE& _state)
{
	state_ = _state;
}

void PlayerChara::PlayAnim(const std::string _anim)
{
	//アニメーション
	animController_->Play(_anim, SPEED_ANIM);
}

const bool PlayerChara::IsLock(void)
{
	return lockState_==LOCK_STATE::LOCKON;
}

void PlayerChara::Damage(const float _pow)
{
	//攻撃力分減らす
	hp_ -= static_cast<int>(_pow);

	uiCntl_->Damage();
	//0以下のとき
	if (hp_ <= 0) {
		//死亡処理
		Deth();
	}
}

void PlayerChara::Jump(void)
{
	if (jumpPow_ > 0.0f)return;
  	jumpPow_ = JUMP_POW;
}

void PlayerChara::DrawDebug(void)
{
	DrawFormatString(0, 40, 0xffffff, "pPos={%.1f,%.1f,%.1f}\npRot={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	/*DrawFormatString(0, 120, 0xffffff, "GoalRot={%.1f,%.1f,%.1f}", goalQua_.x, goalQua_.y, goalQua_.z);
	VECTOR rockPos = SceneManager::GetInstance().GetCamera().GetLockPos();
	float deg = static_cast<float>(Utility::AngleDeg(pos_, VSub(rockPos, pos_)));
	if (pos_.x > rockPos.x)deg = 180.0f + (180.0f - deg);
	DrawFormatString(0, 140, 0xffffff, "RockDeg={%.1f}", deg);

	switch (state_)
	{
	case PlayerChara::STATE::NOMAL:
		DrawString(0, 160,  "NOMAL", 0xffffff);
		break;
	case PlayerChara::STATE::GUARD:
		DrawString(0, 160, "GUARD", 0xffffff);
		break;
	case PlayerChara::STATE::DODGE:
		DrawString(0, 160, "DODGE", 0xffffff);
		break;
	case PlayerChara::STATE::ATTACK:
		DrawString(0, 160, "ATTACK", 0xffffff);
		break;
	default:
		break;
	}

	DrawCupcel();*/

	//collider_->DrawDebugCollider();
}

float PlayerChara::GetToLockDeg(void)
{//ロックオン特有の角度設定
	VECTOR lockPos = SceneManager::GetInstance().GetCamera().GetLockPos();			//ロックオン対象位置
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();	//カメラ角度

	//自分から対象へのベクトル
	auto diff = VSub(lockPos, pos_);
	//角度求める
	return atan2(diff.x, diff.z) - cameraRot.y;
}

void PlayerChara::SetAtkAllert(void)
{
	uiCntl_->ChangeAllert(true);
	allertTime_ = 0;
}

void PlayerChara::HitCollider(std::weak_ptr<Collider> _col)
{
	const float DmgEfcScl = 25.0f;
	const float DmgEfcSpeed = 2.5f;
	const float SwordEfcScl = 50.0f;
	const float SwordEfcSpeed = 1.5;

	using TAG = Collider::COL_TAG;
	//ステージとの衝突
	if (_col.lock()->IsContainsTag(TAG::STAGE)) {
		//本来立つべき位置と現在位置の差分を取得
		VECTOR backVec = VSub(collider_->GetGeometry().GetHitPoint(), pos_);
		VECTOR colNormal = collider_->GetGeometry().GetHitNormal();

		pos_.y = VAdd(pos_, backVec).y;

		gravity_ = { 0.0f,0.0f,0.0f };
		//jumpPow_ = 0.0f;
	}

	//敵の物の場合
	if (_col.lock()->IsContainsTag(TAG::ENEMY)) {
		//攻撃発生者の名前を取得
		auto& atkMaster = _col.lock()->GetMasterName();

		//発動準備中
		if (_col.lock()->IsContainsTag(TAG::PREATTACK)) {
			//警告音
			if (!atkMng_.IsAllert(atkMaster)) {
				//音声の再生
				SoundManager::GetInstance().Play("Allert");
				//警告使用済みに
				CollisionManager::GetInstance().UseAllert(atkMaster);
			}

			//回避状態の場合
			if (state_ == STATE::DODGE) {
				//スロー処理に
				SetIsSlow(true);
				//音声の再生
				SoundManager::GetInstance().Play("JustDodge");
				//判定を使用した
				CollisionManager::GetInstance().UseAttack(atkMaster);
			}
		}
		//攻撃
		else if (_col.lock()->IsContainsTag(TAG::ATTACK)) {
			//ダメージ処理
			Damage(_col.lock()->GetPower());
			auto& efcM = EffectManager::GetInstance();
			efcM.Play(EFC_NAME, "Damage", centerPos_, rot_, DmgEfcScl, DmgEfcSpeed, "Damage");
			efcM.Play(EFC_NAME, "Sword", centerPos_, rot_, SwordEfcScl, SwordEfcSpeed);
			//判定を使用した
			CollisionManager::GetInstance().UseAttack(atkMaster);
		}
	}
}

void PlayerChara::InitAnim(void)
{
	animController_->Add("idle", ANIM_IDLE, AnimationController::PLAY_TYPE::LOOP);
	//攻撃
	animController_->Add("atkFirst", ANIM_ATTACK_FIRST, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("atkSecond", ANIM_ATTACK_SECOND, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("atkSpin", ANIM_ATTACK_SPIN, AnimationController::PLAY_TYPE::NOMAL);
	//防御
	animController_->Add("defStart", ANIM_GUARD_START, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defAtk", ANIM_GUARD_ATTACK, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defHit", ANIM_GUARD_HIT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("defSus", ANIM_GUARD_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
	//その他アクション
	animController_->Add("walk", ANIM_WALK, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("walkB", ANIM_WALK, AnimationController::PLAY_TYPE::RETURN);
	animController_->Add("dushF", ANIM_DUSH_FORWARD, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dushL", ANIM_DUSH_LEFT, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("dushR", ANIM_DUSH_RIGHT, AnimationController::PLAY_TYPE::LOOP);
	animController_->Add("jump", ANIM_JUMP, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodL", ANIM_DODGE_LEFT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodR", ANIM_DODGE_RIGHT, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dodB", ANIM_DODGE_BACK, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("pick", ANIM_PICK_UP, AnimationController::PLAY_TYPE::NOMAL);
	//演出
	animController_->Add("damage", ANIM_DAMAGE, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dethStart", ANIM_DETH_START, AnimationController::PLAY_TYPE::NOMAL);
	animController_->Add("dethSus", ANIM_DETH_SUSTANABLE, AnimationController::PLAY_TYPE::LOOP);
}

void PlayerChara::InitUI(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	uiPos_ = pos_;
	uiPos_.y += 200.0f;
	uiCntl_ = std::make_unique<PlayerUIController>(uiPos_, PALYER_HP);
	uiCntl_->Init(UI_NAME);
}

void PlayerChara::DrawUI(void)
{
	uiCntl_->Draw();
}

void PlayerChara::Move(void)
{
	//移動を行わないとき
	if (moveDir_ == MOVE_DIR::NONE || state_ == STATE::ATTACK) {
		//通常なら
		if (state_ == STATE::NOMAL) {
			//待機アニメーション
			animController_->Play("idle", SPEED_ANIM);
		}
		return;
	}

	InputManager& ins = InputManager::GetInstance();
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetRot();
	VECTOR dir = Utility::VECTOR_ZERO;
	std::string seName = "Walk";

	afterMoveRad_ = 0.0f;

	//移動方向
	if (moveDir_ == MOVE_DIR::FORWARD) {
		dir = cameraRot.GetForward();
		afterMoveRad_ = Utility::Deg2RadF(DEG_FORWARD);
	}
	if (moveDir_ == MOVE_DIR::LEFT) {
		dir = cameraRot.GetLeft();
		afterMoveRad_ = Utility::Deg2RadF(DEG_LEFT);
	}
	if (moveDir_ == MOVE_DIR::BACK) {
		dir = cameraRot.GetBack();
		afterMoveRad_ = Utility::Deg2RadF(DEG_BACK);
	}
	if (moveDir_ == MOVE_DIR::RIGHT) {
		dir = cameraRot.GetRight();
		afterMoveRad_ = Utility::Deg2RadF(DEG_RIGHT);
	}

	//速度設定
	float speed = MOVE_POW;
	//ダッシュのとき
	if (isDush_) {
		speed = DUSH_POW;
		seName = "Dush";
	}
	//ロックオンの時
	if (lockState_ == LOCK_STATE::LOCKON)speed = MOVE_POW;

	dir.y = 0.0f; //上下成分を消す
	//移動処理
	pos_ = VAdd(pos_, VScale(dir, speed));
	//上下の移動が起きない様に
	//ゆくゆくは重力とステージの当たり判定で処理する
	//pos_.y = 0.0f;

	//アニメーション
	//回避中は回避アニメーションを再生しているため他はしない
	if (state_ != STATE::DODGE) {
		animController_->Play(DecideAnim(moveDir_), SPEED_ANIM);
		SoundManager::GetInstance().Play(seName);
	}

	//テキトーな移動制限
	if (Utility::MagnitudeF(pos_) > MOVE_MAX) {
		SetPrevPos();
		return;
	}
}

const std::string PlayerChara::DecideAnim(const MOVE_DIR _dir) const
{
	std::string retAnim = "walk";
	if(isDush_)retAnim = "dushF";

	//ロックオンのとき
	if (lockState_ == LOCK_STATE::LOCKON) {
		if (_dir == MOVE_DIR::LEFT) {
			retAnim = "dushL";
		}
		else if (_dir == MOVE_DIR::RIGHT) {
			retAnim = "dushR";
		}
		else if (_dir == MOVE_DIR::BACK) {
			retAnim = "walkB";
		}
	}
	
	return retAnim;
}
