#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Utility/Utility.h"
#include"../../Object/Character/Player/PlayerChara.h"
#include"../AbilityManager.h"
#include "MagnetCatch.h"

namespace {
	const float ACC_DIREC = 0.02f;	//紐の移動スピード
	const float DIREC_MAX = 1.0f;	//線形補完の最大値(紐の移動を線形補完で行う)
	const float ACC_MOVE = 15.0f;	//物体の移動量
	const float ACC_DEG = 2.0f;		//物体の回転量
	const float ROT_DEG_MAX = 360.0f;	//回転角度の最大値
	const float FLIP_POW = -0.7f;		//ベクトル反転の影響量
}

MagnetCatch::MagnetCatch(AbilityManager& _mng, PlayerChara& _master) :AbilityBase(_mng), master_(_master)
{
	startDirecPos_ = Utility::VECTOR_ZERO;
	goalDirecPos_ = Utility::VECTOR_ZERO;
	nowPos_ = Utility::VECTOR_ZERO;
	relativePos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
	rotationDeg_ = 0.0f;
}

MagnetCatch::~MagnetCatch(void)
{
}

void MagnetCatch::UpdateUse(std::weak_ptr<GimmickObjBase> _obj)
{
	MakeChangeRelativePosition();

	Camera& camera = SceneManager::GetInstance().GetCamera();
	VECTOR cameraRelative = camera.GetC2FRelativeVec();
	cameraRelative.y *= FLIP_POW;
	VECTOR masterPos = master_.GetPos();

	_obj.lock()->SetPos(VAdd(masterPos, cameraRelative));

	VECTOR newObjPos = _obj.lock()->GetPos();

	camera.SetLockPos(newObjPos);

	//プレイヤーの回転
	//VECTOR cameraRot = camera.GetRot().ToEuler();
	//VECTOR master2objDiff = VSub(newObjPos, masterPos);

	//auto rad = atan2(master2objDiff.x, master2objDiff.z) - cameraRot.y;

	//紐の設定
	startDirecPos_ = master_.GetPos();
	goalDirecPos_= newObjPos;
	nowPos_= newObjPos;
}

void MagnetCatch::UpdateDirection(std::weak_ptr<GimmickObjBase> _obj)
{
	//初手のゴール位置設定
	if (!isSetGoalPos_) {
		if (_obj.expired()) {
			VECTOR screenPos = AbilityManager::RETICLE_POS;
			VECTOR woldPos = ConvScreenPosToWorldPos(screenPos);
			goalDirecPos_ = woldPos;
		}
		else {
			goalDirecPos_ = _obj.lock()->GetPos();
		}
		isSetGoalPos_ = true;
	}

	//演出終了
	if (direcStep_ > DIREC_MAX) {
		//対象がないとき
		if (_obj.expired()) {
			manager_.ChangeState(AbilityManager::STATE::REDY);
		}
		else {
			//使用に遷移
			manager_.ChangeState(AbilityManager::STATE::USE);
			goalDirecPos_ = _obj.lock()->GetPos();
			relativePos_ = VSub(goalDirecPos_, startDirecPos_);
			//カメラ設定
			auto& camera=SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::MIRROR);
			camera.SetLockPos(goalDirecPos_);

			//キャラクターの設定
			//master_.SetIsRotation(false);
			master_.ChangeLockState(true);
		}
	}

	//演出更新
	startDirecPos_ = master_.GetPos();
	nowPos_=Utility::Lerp(startDirecPos_, goalDirecPos_, direcStep_);
	direcStep_ += ACC_DIREC;
}

void MagnetCatch::Draw(void)
{
	auto state = manager_.GetAbilityState();
	if (state == AbilityManager::STATE::DIRECTION || state == AbilityManager::STATE::USE) {
		const int debugCol = 0x0055ff;
		const float debugScl = 10.0f;
		const int divNum = 8;

		DrawLine3D(startDirecPos_, nowPos_, debugCol);
		DrawSphere3D(startDirecPos_, debugScl, divNum, debugCol, debugCol, false);
		DrawSphere3D(nowPos_, debugScl, divNum, debugCol, debugCol, false);
	}

	//デバッグ
	DrawFormatString(50, 300, 0xff0000, "GOAL={%.1f,%.1f,%.1f}", goalDirecPos_.x, goalDirecPos_.y, goalDirecPos_.z);
}

void MagnetCatch::ResetAbility(void)
{
	startDirecPos_ = Utility::VECTOR_ZERO;
	goalDirecPos_ = Utility::VECTOR_ZERO;
	nowPos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

void MagnetCatch::MakeChangeRelativePosition(void)
{
	InputManager& ins = InputManager::GetInstance();

	//if (ins.IsPressed("subUp"))
	//{
	//	relativePos_.y += ACC_MOVE;
	//}
	//if (ins.IsPressed("subDown"))
	//{
	//	relativePos_.y -= ACC_MOVE;
	//}
	//if (ins.IsPressed("subLeft"))
	//{
	//	//relativePos_.x -= ACC_MOVE;
	//	rotationDeg_ -= ACC_DEG;
	//	if (rotationDeg_ < 0.0f) {
	//		rotationDeg_ = ROT_DEG_MAX;
	//	}
	//}
	//if (ins.IsPressed("subRight"))
	//{
	//	//relativePos_.x += ACC_MOVE;
	//	rotationDeg_ += ACC_DEG;
	//	if (rotationDeg_ > ROT_DEG_MAX) {
	//		rotationDeg_ = 0.0f;
	//	}
	//}

	//前後の入力を決めたら相対座標のXをいじる

	Rotation();
}

void MagnetCatch::Rotation(void)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetAngle();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + Utility::Deg2RadF(rotationDeg_), Utility::AXIS_Y);

	magRotY_ = axis;
}
