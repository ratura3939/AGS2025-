#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "SceneManager.h"
#include "InputManager.h"
//#include "../Object/Character/PlayableChara/PlayerBase.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	currentMode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	focusPos_ = { 0.0f, 0.0f, 0.0f };
	rockPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();

	stepReset_ = 0.0f;
	isReset_ = true;

	followObject_.pos = { 0.0f, 0.0f, 0.0f };
	followObject_.quaRot = Quaternion::Identity();
	start_.pos = { 0.0f, 0.0f, 0.0f };
	start_.quaRot = Quaternion::Identity();
	goal_.pos = { 0.0f, 0.0f, 0.0f };
	goal_.quaRot = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	//カメラの初期設定
	SetDefault();


}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case MODE::NONE:
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

	case MODE::ROCKON:
		SetBeforeDrawRockOn();
		break;

	case MODE::FOLLOW_SPRING:
		break;

	case MODE::SHAKE:
		SetBeforeDrawShake();
		break;

	case MODE::RESET:
		SetBeforeDrawReset();
		break;
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		focusPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
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

	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;


	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_N)) {
		ChangeMode(MODE::RESET);
	}

	//追従対象からカメラまでの相対座標(カメラの回転情報をもとに相対座標を回転させる)
	VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	//カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	//targetPos_ = VAdd(pos_, relativeTPos);

	//カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

void Camera::SetBeforeDrawRockOn(void)
{

	//TODO
	//違和感が残っているので後で見直す

	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;
	//離れている距離
	VECTOR distance = VSub(rockPos_,followPos);


	//追従対象からカメラまでの相対座標(カメラの回転情報をもとに相対座標を回転させる)
	float disMag = Utility::MagnitudeF(distance);

	VECTOR relative = { 0.0f,disMag*0.25f,-disMag};

	VECTOR relativeCPos = rot_.PosAxis(relative);

	//カメラ位置の更新
	pos_ = VAdd(focusPos_, relativeCPos);

	if (pos_.y < UNDERLIMIT_Y)pos_.y = UNDERLIMIT_Y;


	//注視点の更新
	//ロックオン中の注視点は追従対象とロックオン対象の中間地点にある。
	focusPos_ = VAdd(followPos,VScale(distance, 0.5f));

	//カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
	//cameraUp_ = followRot.PosAxis(Quaternion::Identity().GetUp());

	//初動時のみに発動する
	//カメラの初期ゴールを計算結果で算出した場所にする
	if (!isReset_) {
		ChangeMode(MODE::RESET);
		goal_.pos = pos_;
		goal_.quaRot = followObject_.quaRot;
	}
}

void Camera::SetBeforeDrawShake(void)
{
	// 一定時間カメラを揺らす
	stepShake_ -= SceneManager::GetInstance().GetDeltaTime();

	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::FOLLOW_SPRING);
		return;
	}

	// -1.0f～1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);

	// -1000.0f～1000.0f
	f *= 1000.0f;

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

	//float pow = WIDTH_SHAKE * sinf(stepShake_ * SPEED_SHAKE);
	//VECTOR velocity = VScale(shakeDir_, pow);
	//VECTOR newPos = VAdd(defaultPos_, velocity);
	//pos_ = newPos;

}

void Camera::SetBeforeDrawReset(void)
{
	stepReset_ += RESET_STEP;
	//終了条件
	if (stepReset_ >= RESET_TIME) {
		ChangeMode(currentMode_);
		isReset_ = true;
		return;
	}

	rot_ = Quaternion::Slerp(start_.quaRot, goal_.quaRot, stepReset_);
	pos_ = Utility::Lerp(start_.pos, goal_.pos, stepReset_);
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

const VECTOR Camera::GetPos(void) const
{
	return pos_;
}

const Quaternion Camera::GetRot(void) const
{
	return rot_;
}

void Camera::ChangeMode(MODE mode)
{

	//カメラの初期設定
	//カメラを揺らす前の位置で揺れるようにしたいため外している
	//SetDefault();
	
	if (mode == MODE::RESET)currentMode_ = mode_;

	//カメラモードの変更
  	mode_ = mode;

	isReset_ = false;

	//変更時の初期化処理
	switch (mode_)
	{
	case MODE::FIXED_POINT:
		break;
	case MODE::FREE:
		break;
	case MODE::FOLLOW:
		break;
	case MODE::FOLLOW_SPRING:
		break;
	case MODE::SHAKE:
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm({ 0.7f, 0.7f ,0.0f });
		defaultPos_ = pos_;
	case MODE::RESET:
		stepReset_ = 0.0f;
		start_.pos = pos_;
		start_.quaRot = rot_;
		goal_.pos = VAdd(followObject_.pos, RELATIVE_F2C_POS_FOLLOW);
		goal_.quaRot = followObject_.quaRot;
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

void Camera::SetFocusPos(const VECTOR& _focus)
{
	focusPos_ = _focus;
}

void Camera::SetRockPos(const VECTOR& _rock)
{
	rockPos_ = _rock;
}

const VECTOR Camera::GetRockPos(void) const
{
	return rockPos_;
}

const Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

void Camera::DrawDebug(void)
{
	DrawFormatString(0, 0, 0xffffff, "cPOS={%.1f,%.1f,%.1f}\ncROT={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	DrawFormatString(0, 100, 0xffffff, "FCPOS={%.1f,%.1f,%.1f}", focusPos_.x, focusPos_.y, focusPos_.z);
	DrawSphere3D(focusPos_, 8, 10, 0x00ff00, 0x00ff00, false);
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
	auto& ins = InputManager::GetInstance();
	//回転軸と量を決める
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = Utility::VECTOR_ZERO;
	if (ins.IsPressed("upSub")) { axisDeg.x = -1.0f; }
	if (ins.IsPressed("downSub")) { axisDeg.x = 1.0f; }
	if (ins.IsPressed("leftSub")) { axisDeg.y = 1.0f; }
	if (ins.IsPressed("rightSub")) { axisDeg.y = -1.0f; }


	//カメラ座標を中心として、注視点を回転させる
	if (!Utility::EqualsVZero(axisDeg))
	{
		//今回の回転量を合成
		//今回はY軸のみの回転
		Quaternion rotPow;
			/*rotPow = rotPow.Mult(
				Quaternion::AngleAxis(
					Utility::Deg2RadF(axisDeg.z), Utility::AXIS_Z));*/
			/*rotPow = rotPow.Mult(
				Quaternion::AngleAxis(
					Utility::Deg2RadF(axisDeg.x), Utility::AXIS_X));*/
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				Utility::Deg2RadF(axisDeg.y), Utility::AXIS_Y));

		//カメラの回転の今回の回転量を加える（合成）
		rot_ = rot_.Mult(rotPow);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();
	}
}

