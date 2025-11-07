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
	const float ACC_MOVE = 5.0f;	//物体の移動量
	const float ACC_DEG = 2.0f;		//物体の回転量
	const float ROT_DEG_MAX = 360.0f;	//回転角度の最大値
	const float FLIP_POW = -0.7f;		//ベクトル反転の影響量
	const float MAX_RELATIVE_Z = 500.0f;	//相対座標の最大値
}

MagnetCatch::MagnetCatch(AbilityManager& _mng, PlayerChara& _master) :AbilityBase(_mng), master_(_master)
{
	startDirecPos_ = Utility::VECTOR_ZERO;
	goalDirecPos_ = Utility::VECTOR_ZERO;
	nowPos_ = Utility::VECTOR_ZERO;
	relativePosZ_ = 0.0f;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

MagnetCatch::~MagnetCatch(void)
{
}

void MagnetCatch::UpdateUse(std::weak_ptr<GimmickObjBase> _obj)
{
	MakeChangeRelativePosition();

	Camera& camera = SceneManager::GetInstance().GetCamera();
	VECTOR masterPos = master_.GetPos();
	//カメラ→プレイヤーのベクトル
	VECTOR cameraRelative = camera.GetC2FRelativeVec();
	//反転処理
	cameraRelative.y *= FLIP_POW;
	
	//相対座標の加算
	VECTOR addVec = master_.GetQua().PosAxis({ 0.0f,0.0f,relativePosZ_ });
	VECTOR objRelative = VAdd(cameraRelative, addVec);

	_obj.lock()->SetPos(VAdd(masterPos, objRelative));

	//ターゲット位置の更新
	VECTOR newObjPos = _obj.lock()->GetPos();
	camera.SetLockPos(newObjPos);

	//紐の設定
	startDirecPos_ = masterPos;
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
			_obj.lock()->AffectedMagnet();
			//カメラ設定
			auto& camera=SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::MIRROR);
			camera.SetLockPos(goalDirecPos_);

			//キャラクターの設定
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
	relativePosZ_ = 0.0f;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

void MagnetCatch::EndAbility(void)
{
	//キャラクターの設定
	master_.ChangeLockState(false);
}

void MagnetCatch::MakeChangeRelativePosition(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsPressed("push")) {
		relativePosZ_ += ACC_MOVE;
		if(relativePosZ_ > MAX_RELATIVE_Z) {
			relativePosZ_ = MAX_RELATIVE_Z;
		}
	}
	else if (ins.IsPressed("pull")) {
		relativePosZ_ -= ACC_MOVE;
		if (relativePosZ_ < 0.0f) {
			relativePosZ_ = 0.0f;
		}
	}
}
