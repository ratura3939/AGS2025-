#include"../../Application.h"
#include"../../Utility/Utility.h"
#include"SceneManager.h"
#include"InputManager.h"
#include "Camera.h"

//シングルトン化(インスタンスの初期化)
Camera* Camera::instance_ = nullptr;

void Camera::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Camera();	//インスタンス生成
	}
	instance_->Init();
}

Camera& Camera::GetInstance(void)
{
	return *instance_;
}

Camera::Camera(void)
{
	
	pos_ = { 0.0f,0.0f,0.0f };
	localCenterPos_ = { 0.0f,0.0f,0.0f };
	targetPos_ = { 0.0f,0.0f,0.0f };
	mapSize_ = { 10000.0f,10000.0f ,0.0f};
}

Camera::~Camera(void)
{
}

void Camera::DrawDebug(void)
{
	DrawFormatString(300, 0, 0xff0000, "CPOS = { %.1f ,%.1f }", pos_.x, pos_.y, true);
	DrawFormatString(300, 20, 0xff0000, "TPOS = { %.1f ,%.1f }", targetPos_.x, targetPos_.y, true);
}

bool Camera::Init(void)
{
	localCenterPos_ = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 ,0.0f};
	mode_ = MODE::NONE;
	return true;
}

void Camera::Update(void)
{
	switch (mode_)
	{
	case Camera::MODE::TO_FOLLOW:
		MoveCameraToFollow();
		break;
	case Camera::MODE::FREE:
		MoveCameraFree();
		break;
	case Camera::MODE::FOLLOW:
		MoveCameraFollow();
		break;
	default:
		break;
	}
}

void Camera::Relese(void)
{
	delete instance_;
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
		pos_ = { 0.0f,0.0f,0.0f };
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::SHAKE:
		break;
	}

}

void Camera::SetTargetPos(const VECTOR pPos)
{
	targetPos_ = pPos;
}

void Camera::SetMapSize(const VECTOR mapsize)
{
	mapSize_ = mapsize;
}

const VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::SwapModeFree2Follow(void)
{
	if (mode_ == MODE::FOLLOW) {
		mode_ = MODE::FREE;
		return;
	}
	if (mode_ == MODE::FREE) {
		mode_ = MODE::TO_FOLLOW;
	}
}

void Camera::MoveCameraFree(void)
{
	auto& ins = InputManager::GetInstance();
	VECTOR moveDir = { 0.0f,0.0f,0.0f };

	if (SceneManager::GetInstance().GetController() == SceneManager::CNTL::KEY)
	{
		//キーボード入力
		if (ins.IsNew(KEY_INPUT_W)) {
			moveDir.y -= MOVE_SPEED;
		}
		if (ins.IsNew(KEY_INPUT_A)) {
			moveDir.x -= MOVE_SPEED;
		}
		if (ins.IsNew(KEY_INPUT_S)) {
			moveDir.y += MOVE_SPEED;
		}
		if (ins.IsNew(KEY_INPUT_D)) {
			moveDir.x += MOVE_SPEED;
		}
	}
	//パッド入力
	else {
		auto stickX = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyRX;
		auto stickY = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyRY;

		if (stickY < -SceneManager::STICK_START_POW) {
			moveDir.y -= MOVE_SPEED;
		}
		if (stickX < -SceneManager::STICK_START_POW) {
			moveDir.x -= MOVE_SPEED;
		}
		if (stickY > SceneManager::STICK_START_POW) {
			moveDir.y += MOVE_SPEED;
		}
		if (stickX > SceneManager::STICK_START_POW) {
			moveDir.x += MOVE_SPEED;
		}
	}

	//移動
	pos_ = VAdd(pos_, moveDir);
}

void Camera::MoveCameraFollow(void)
{
	//カメラ左上（始点）とプレイヤー（終点）のベクトル
	auto diff = VSub(targetPos_, pos_);
	//現在のカメラ位置から相対的に
	auto scrCenter = VAdd(pos_, localCenterPos_);

	auto cameraDir = VSub(scrCenter, pos_);

	//カメラ差分移動(大きいー小さい）
	//左移動
	if (diff.x < cameraDir.x) { pos_.x -= cameraDir.x - diff.x; }
	//右移動
	if (diff.x > cameraDir.x) { pos_.x += diff.x - cameraDir.x; }
	//上移動
	if (diff.y < cameraDir.y) { pos_.y -= cameraDir.y - diff.y; }
	//下移動
	if (diff.y > cameraDir.y) { pos_.y += diff.y - cameraDir.y; }

	//移動制限
	if (pos_.x < 0) { pos_.x = 0; }
	if (pos_.x + Application::SCREEN_SIZE_X > mapSize_.x) { pos_.x = mapSize_.x - Application::SCREEN_SIZE_X; }
	if (pos_.y < 0) { pos_.y = 0; }
	if (pos_.y + Application::SCREEN_SIZE_Y > mapSize_.y) { pos_.y = mapSize_.y - Application::SCREEN_SIZE_Y; }
}

void Camera::MoveCameraToFollow(void)
{
	//目標を中央にするときの目標のカメラ位置(カメラ位置は左上の点)
	VECTOR goal = VSub(targetPos_, localCenterPos_);
	
	//目標と現在位置の差分
	//現在位置(始点)　目標(終点)
	VECTOR diff = VSub(goal, pos_);
	
	//横軸の移動
	if (diff.x > 0.0f) {
		//右移動
		pos_.x += MOVE_SPEED;
		//移動後通り過ぎたら強制的に目標位置に設定
		//右移動なので超過している場合はX軸が大きかった時
		if (pos_.x >= goal.x)pos_.x = goal.x;
	}
	if (diff.x < 0.0f) {
		//左移動
		pos_.x -= MOVE_SPEED;
		//移動後通り過ぎたら強制的に目標位置に設定
		//右移動なので超過している場合はX軸が小さかった時
		if (pos_.x <= goal.x)pos_.x = goal.x;
	}

	//縦軸の移動
	if (diff.y > 0.0f) {
		//下移動
		pos_.y += MOVE_SPEED;
		//移動後通り過ぎたら強制的に目標位置に設定
		//右移動なので超過している場合はX軸が大きかった時
		if (pos_.y >= goal.y)pos_.y = goal.y;
	}
	if (diff.y < 0.0f) {
		//上移動
		pos_.y -= MOVE_SPEED;
		//移動後通り過ぎたら強制的に目標位置に設定
		//右移動なので超過している場合はX軸が小さかった時
		if (pos_.y <= goal.y)pos_.y = goal.y;
	}


	//終了条件
	if (Utility::Equals(pos_, goal))mode_ = MODE::FOLLOW;
}
