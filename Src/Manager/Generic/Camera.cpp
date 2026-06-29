#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Camera.h"

namespace {
	const float LERP_SPEED = 0.1f;		//補完スピード
	const float LERP_MAX = 1.0f;		//補完上限値
	const float HALF_DISTANCE = 0.5f;	//中間地点(ロックオン時の注視点に使用)
	const float WALL_LERP_SPEED = 1.0f;	//壁補正の補完スピード
	const VECTOR SHAKE_DIR = { 0.7f, 0.7f ,0.0f };	//揺れ方向
	const float LERP_STEP_FOUCUS_RESET = 0.8f;	//カメラリセット時の補完スピード
	const float LERP_STEP_AUTOMOVE = 0.01f;		//カメラ自動移動時の補完スピード
}

Camera::Camera(void)
	:followObject_({ Utility::VECTOR_ZERO, Quaternion::Identity() })
	,resetStartPos_({ Utility::VECTOR_ZERO, Quaternion::Identity() })
	,resetGoalPos_({ Utility::VECTOR_ZERO, Quaternion::Identity() })
	,stepReset_(0.0f)
	,isReset_(true)
	,currentMode_(MODE::MAX)
	,returnMode_(MODE::MAX)
	,pos_(Utility::VECTOR_ZERO)
	,lockPos_(Utility::VECTOR_ZERO)
	,lockOnGoalPos_(Utility::VECTOR_ZERO)
	,lockOnDistanceMin_(0.0f)
	,prevGoalPos_(Utility::VECTOR_ZERO)
	,focusPos_(Utility::VECTOR_ZERO)
	,goalFocusPos_(Utility::VECTOR_ZERO)
	,goalDirecPos_(Utility::VECTOR_ZERO)
	,idealPos_(Utility::VECTOR_ZERO)
	,adjustedPos_(Utility::VECTOR_ZERO)
	,cameraUp_(Utility::VECTOR_ZERO)
	,rot_(Quaternion::Identity())
	,rotSpeed_(MAX_ROT_SPEED)
	,angles_(Utility::VECTOR_ZERO)
	,rotOutX_(Quaternion::Identity())
	,stepShake_(0.0f)
	,finishShake_(false)
	,defaultPos_(Utility::VECTOR_ZERO)
	,shakeDir_(SHAKE_DIR)
	,lerpStep_(0.0f)
	,c2fRelative_(Utility::VECTOR_ZERO)
{
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	//カメラの初期設定
	SetDefault();
	collider_ = std::make_unique<CameraCollider>(*this);
	collider_->Init();
}

void Camera::Update(void)
{
	collider_->Update();
}

void Camera::SetBeforeDraw(void)
{
	//クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	lerpStep_ += LERP_SPEED;
	if (lerpStep_ > LERP_MAX)lerpStep_ = LERP_MAX;

	switch (currentMode_)
	{
	case MODE::MAX:
		SetBeforeDrawFollow();
		break;

	case MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case MODE::FREE:
		SetBeforeDrawFree();
		break;
	
	case MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case MODE::LOCKON:
		SetBeforeDrawLockOn();
		break;
	case MODE::SHAKE:
		SetBeforeDrawShake();
		break;

	case MODE::RESET:
		SetBeforeDrawReset();
		break;

	case MODE::AUTO_MOVE:
		SetBeforeDrawAutoMove();
		break;

	case MODE::MIRROR:
		SetBeforeDrawMirror();
		break;
	}

	// FOLLOW・LOCKON・NONE時にレイキャストによるカメラ位置補正を適用
	if (currentMode_ == MODE::FOLLOW || currentMode_ == MODE::LOCKON || currentMode_ == MODE::MAX) {
		collider_->UpdateRayCast();
		pos_ = Utility::Lerp(pos_, adjustedPos_, WALL_LERP_SPEED);
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		focusPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	c2fRelative_ = VSub(followObject_.pos, pos_);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	Rotation();
}

void Camera::SetBeforeDrawFollow(void)
{
	//回転
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	auto& ins = InputManager::GetInstance();
	if (ins.IsTrigerrDown("rock")) {
		ChangeMode(MODE::RESET);
		resetGoalPos_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(RELATIVE_F2C_POS_FOLLOW));
		resetGoalPos_.quaRot = followObject_.quaRot;
		return;
	}

	//追従対象までの距離ベクトルを回転させ相対座標を生成
	VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//カメラ位置の更新(追従対象位置から相対座標を足す)
	VECTOR gPos = VAdd(followPos, relativeCPos);

	if (fabs(Utility::MagnitudeF(gPos) - Utility::MagnitudeF(pos_)) <= 10.0f) {
		lerpStep_ = NO_LERP;
	}
	idealPos_ = gPos;

	//注視点までの距離ベクトルを回転させ相対座標を生成
	VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	focusPos_ = VAdd(followPos, relativeTPos);

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawLockOn(void)
{
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;
	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	//ロックオン対象と追従対象の離れている距離
	VECTOR distance = VSub(lockPos_, followPos);

	//離れる距離を数値化
	float disMag = Utility::MagnitudeF(distance);
	//最低限の値を下回っていたら
	if (disMag <= lockOnDistanceMin_) {
		//最低限の値を入れる
		disMag = lockOnDistanceMin_;
	}

	//カメラ位置調整(カメラは後方位置に。Y方向は距離に応じて高さを変える。)
	VECTOR relative = { 0.0f,disMag * ROCK_MAGNIFICATION_Y,-disMag };
	//カメラの回転情報をもとに相対座標を回転させる
	VECTOR relativeCPos = rot_.PosAxis(relative);

	//初動時のみに発動する
	//カメラの初期ゴールを計算結果で算出した場所にする
	if (!isReset_) {
		ChangeMode(MODE::RESET);
		resetGoalPos_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(relative));
		resetGoalPos_.quaRot = followObject_.quaRot;
		return;
	}

	//注視点の更新
	//ロックオン中の注視点は追従対象とロックオン対象の中間地点にある。
	goalFocusPos_ = VAdd(followPos, VScale(distance, HALF_DISTANCE));
	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, lerpStep_);

	//カメラ位置の更新
	prevGoalPos_ = lockOnGoalPos_;
	lockOnGoalPos_ = VAdd(focusPos_, relativeCPos);

	//pos_ = Utility::Lerp(pos_, lockOnGoalPos_, lerpStep_);
	idealPos_ = lockOnGoalPos_;

	//ある程度の高さは保つ
	if (pos_.y < UNDER_LIMIT_Y)pos_.y = UNDER_LIMIT_Y;
	if (pos_.y > HIGHT_LIMIT_Y)pos_.y = HIGHT_LIMIT_Y;

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawShake(void)
{
	stepShake_ -= 0.01f;

	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::FIXED_POINT);
		finishShake_ = true;
		return;
	}

	// -1.0f～1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);

	const float SHAKE_DIGIT_SCALE = 1000.0f;

	// -1000.0f～1000.0f
	f *= SHAKE_DIGIT_SCALE;

	// -1000 or 1000
	int d = static_cast<int>(f);

	// 0 or 1
	int shake = d % 2;

	// 0 or 2
	shake *= 2;

	// -1 or 1
	shake -= 1;

	// 移動量
	VECTOR velocity = VScale(shakeDir_, (float)(shake)*WIDTH_SHAKE);

	// 移動先座標
	 pos_ = VAdd(defaultPos_, velocity);
}

void Camera::SetBeforeDrawReset(void)
{
	stepReset_ += RESET_STEP;
	if (stepReset_ >= RESET_TIME) {
		lerpStep_ = NO_LERP;
		idealPos_ = pos_;
		adjustedPos_ = pos_;

		ChangeMode(returnMode_);
		isReset_ = true;
		angles_ = Utility::VECTOR_ZERO;

		VECTOR finishEuler = rot_.ToEuler();
		angles_.x = finishEuler.x;
		angles_.y = finishEuler.y;

		return;
	}

	// 球面補間
	rot_ = Quaternion::Slerp(resetStartPos_.quaRot, resetGoalPos_.quaRot, stepReset_);
	pos_ = VAdd(followObject_.pos, rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW));

	// 注視点を追従対象に追従させる（毎フレーム更新）
	goalFocusPos_ = followObject_.pos;
	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, LERP_STEP_FOUCUS_RESET);

	// angleの逆算
	VECTOR currentEuler = rot_.ToEuler();
	angles_.x = currentEuler.x;
	angles_.y = currentEuler.y;

	// カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawAutoMove(void)
{
	//目標位置まで移動する
	//終了の判定は呼び出した側で行う
	pos_ = Utility::Lerp(pos_, goalDirecPos_, LERP_STEP_AUTOMOVE);

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawMirror(void)
{
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;
	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	//ロックオン対象と追従対象の離れている距離
	VECTOR distance = VSub(lockPos_, followPos);

	//離れる距離を数値化
	float disMag = Utility::MagnitudeF(distance);

	//最低限の値を下回っていたら
	if (disMag <= ROCK_DISTANCE_MIN) {
		//最低限の値を入れる
		disMag = ROCK_DISTANCE_MIN;
	}

	//カメラ位置調整(カメラは後方位置に。Y方向は距離に応じて高さを変える。)
	VECTOR relative = { 0.0f,disMag * MIRROR_MAGNIFICATION_Y,-disMag * MIRROR_MAGNIFICATION_Z };
	//カメラの回転情報をもとに相対座標を回転させる
	VECTOR relativeCPos = rot_.PosAxis(relative);

	//初動時のみに発動する
	//カメラの初期ゴールを計算結果で算出した場所にする
	if (!isReset_) {
		ChangeMode(MODE::RESET);
		resetGoalPos_.pos = VAdd(followObject_.pos, followObject_.quaRot.PosAxis(relative));
		resetGoalPos_.quaRot = followObject_.quaRot;
		return;
	}

	//注視点の更新
	//ロックオン中の注視点は追従対象とロックオン対象の中間地点にある。
	goalFocusPos_ = VAdd(followPos, VScale(distance, 0.5f));
	focusPos_ = Utility::Lerp(focusPos_, goalFocusPos_, 0.2f);

	//カメラ位置の更新
	prevGoalPos_ = lockOnGoalPos_;
	lockOnGoalPos_ = VAdd(focusPos_, relativeCPos);

	pos_ = Utility::Lerp(pos_, lockOnGoalPos_, lerpStep_);

	//ある程度の高さは保つ
	if (pos_.y < UNDER_LIMIT_Y)pos_.y = UNDER_LIMIT_Y;
	if (pos_.y > HIGHT_LIMIT_Y)pos_.y = HIGHT_LIMIT_Y;

	//カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

const VECTOR& Camera::GetPos(void) const
{
	return pos_;
}

const Quaternion& Camera::GetRot(void) const
{
	return rot_;
}

const VECTOR& Camera::GetAngle(void) const
{
	return angles_;
}

const float Camera::GetRotSpeed(void) const
{
	return rotSpeed_;
}

void Camera::SetRotSpeed(const float _speed)
{
	rotSpeed_ = _speed;
}

void Camera::ChangeMode(MODE mode)
{
	//リセット時は現在のモードを保存しておく
	if (mode == MODE::RESET)returnMode_ = currentMode_;

	//カメラモードの変更
  	currentMode_ = mode;

	isReset_ = false;
	lerpStep_ = 0.0f;

	//変更時の初期化処理
	switch (currentMode_)
	{
	case MODE::FIXED_POINT:
		break;

	case MODE::FREE:
		break;

	case MODE::FOLLOW:
		adjustedPos_ = pos_;
		break;

	case MODE::SHAKE:
		finishShake_ = false;
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm(SHAKE_DIR);
		defaultPos_ = pos_;
		break;

	case MODE::RESET:
	{
		stepReset_ = 0.0f;
		resetStartPos_.pos = pos_;
		resetStartPos_.quaRot = rot_;
		goalFocusPos_ = followObject_.pos;

		//回転の同期
		VECTOR currentEuler = rot_.ToEuler();
		angles_.x = currentEuler.x;
		angles_.y = currentEuler.y;
	}
		break;

	case MODE::AUTO_MOVE:
		angles_ = Utility::VECTOR_ZERO;
		rot_ = Quaternion::Identity();
		break;

	case MODE::LOCKON:
		break;

	case MODE::MIRROR:
		break;
	}

}

void Camera::SetFollow(const VECTOR _pos, const Quaternion _qua)
{
	followObject_.pos = _pos;
	followObject_.quaRot = _qua;
}

void Camera::SetPos(const VECTOR& pos, const VECTOR& target)
{
	pos_ = pos;
	focusPos_ = target;
}

void Camera::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void Camera::SetFocusPos(const VECTOR& _focus)
{
	focusPos_ = _focus;
}

void Camera::SetGoalFocusPos(const VECTOR& _focus)
{
	goalFocusPos_ = _focus;
}

void Camera::SetLockPos(const VECTOR& _lock)
{
	lockPos_ = _lock;
}

void Camera::SetGoalDirecPos(const VECTOR& _goal)
{
	goalDirecPos_ = _goal;
}

const VECTOR& Camera::GetLockPos(void) const
{
	return lockPos_;
}

void Camera::ResetCollider(void)
{
	collider_->SetCollider();
}

const Camera::MODE& Camera::GetMode(void) const
{
	return currentMode_;
}

void Camera::SetDefault(void)
{
	//カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	//注視点
	focusPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//カメラはX軸に傾いているが、
	//この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();
}

void Camera::Rotation(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsPressed("subUp"))
	{
		angles_.x -= rotSpeed_;
		if (angles_.x <= LIMIT_X_DW_RAD)
			angles_.x = LIMIT_X_DW_RAD;
	}
	if (ins.IsPressed("subDown"))
	{
		angles_.x += rotSpeed_;
		if (angles_.x >= LIMIT_X_UP_RAD)
			angles_.x = LIMIT_X_UP_RAD;
	}
	if (ins.IsPressed("subLeft"))
	{
		angles_.y -= rotSpeed_;
	}
	if (ins.IsPressed("subRight"))
	{
		angles_.y += rotSpeed_;
	}

	//カメラ座標を中心として、注視点を回転させる
	if (!Utility::EqualsVZero(angles_))
	{
		// 正面から設定されたY軸分、回転させる
		rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

		// 正面から設定されたX軸分、回転させる
		rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
		// カメラの上方向
		cameraUp_ = rot_.GetUp();
	}
}

