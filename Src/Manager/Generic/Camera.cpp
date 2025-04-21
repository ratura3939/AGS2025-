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
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	//カメラの初期設定
	SetDefault();

	moveSpeed_ = 0.0f;

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
	case Camera::MODE::NONE:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::ROCKON:
		SetBeforeDrawRockOn();
		break;

	case Camera::MODE::FOLLOW_SPRING:
		break;

	case Camera::MODE::SHAKE:
		SetBeforeDrawShake();
		break;
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
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
	auto& ins = InputManager::GetInstance();

	ProcessMove();

	Decelerate(MOVE_DEC);

	Move();

}

void Camera::SetBeforeDrawFollow(void)
{

	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

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
	Rotation();

	//追従対象の位置
	VECTOR followPos = followObject_.pos;

	//追従対象の向き
	Quaternion followRot = followObject_.quaRot;

	//カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
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

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	//カメラの初期設定
	//カメラを揺らす前の位置で揺れるようにしたいため外している
	//SetDefault();
	
	//カメラモードの変更
  	mode_ = mode;

	//変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::SHAKE:
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm({ 0.7f, 0.7f ,0.0f });
		defaultPos_ = pos_;
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
	targetPos_ = target;
}

void Camera::SetTargetPos(const VECTOR& _target)
{
	targetPos_ = _target;
}

const Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

void Camera::DrawDebug(void)
{
	DrawFormatString(0, 0, 0xffffff, "cPOS={%.1f,%.1f,%.1f}\ncROT={%.1f,%.1f,%.1f}", pos_.x, pos_.y, pos_.z, rot_.x, rot_.y, rot_.z);
	DrawSphere3D(targetPos_, 8, 10, 0x00ff00, 0x00ff00, false);
}

void Camera::SetDefault(void)
{

	//カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	//注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//カメラはX軸に傾いているが、
	//この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();

	velocity_ = Utility::VECTOR_ZERO;

}

void Camera::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//移動
	//moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = Utility::DIR_F; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = Utility::DIR_B; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = Utility::DIR_L; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = Utility::DIR_R; Accele(MOVE_ACC); }

	Rotation();
}

void Camera::Move(void)
{
	//移動処理
	if (!Utility::EqualsVZero(moveDir))
	{
		//移動 = 座標 + 移動量
		//移動量 = 方向 * スピード 

		//入力された方向をカメラの回転情報を使って、
		//カメラの進行方向に変換する
		VECTOR direction = rot_.PosAxis(moveDir);

		//移動量
		VECTOR movePow = VScale(direction, moveSpeed_);

		//移動処理
		pos_ = VAdd(pos_, movePow);

		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Camera::Rotation(void)
{
	auto& ins = InputManager::GetInstance();
	//回転軸と量を決める
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = Utility::VECTOR_ZERO;
	/*if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = 1.0f; }*/
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = 1.0f; }


	//カメラ座標を中心として、注視点を回転させる
	if (!Utility::EqualsVZero(axisDeg))
	{
		//今回の回転量を合成
		//今回はY軸のみの回転
		Quaternion rotPow;
		/*	rotPow = rotPow.Mult(
				Quaternion::AngleAxis(
					Utility::Deg2RadF(axisDeg.z), Utility::AXIS_Z));
			rotPow = rotPow.Mult(
				Quaternion::AngleAxis(
					Utility::Deg2RadF(axisDeg.x), Utility::AXIS_X));*/
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				Utility::Deg2RadF(axisDeg.y), Utility::AXIS_Y));

		//カメラの回転の今回の回転量を加える（合成）
		rot_ = rot_.Mult(rotPow);

		//注視点の相対座標を回転させる
		//VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//注視点更新
		//targetPos_ = VAdd(pos_, rotLocalPos);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();
	}
}

void Camera::Accele(float speed)
{
	moveSpeed_ += speed;

	//速度制限(右方向)
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	//速度制限(左方向)
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}

void Camera::Decelerate(float speed)
{
	//右方向の移動を減速させる
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}

	//左方向の移動を減速させる
	if (moveSpeed_ < 0.0f)
	{
		moveSpeed_ += speed;
		if (moveSpeed_ > 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
}