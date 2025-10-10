#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Utility/Utility.h"
#include"../AbilityManager.h"
#include "MagnetCatch.h"

namespace {
	const float ACC_DIREC = 0.02f;
	const float DIREC_MAX = 1.0f;
	const float ACC_MOVE = 5.0f;
}

MagnetCatch::MagnetCatch(AbilityManager& _mng):AbilityBase(_mng)
{
	startDirecPos_ = Utility::VECTOR_ZERO;
	goalDirecPos_ = Utility::VECTOR_ZERO;
	nowPos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

MagnetCatch::~MagnetCatch(void)
{
}

void MagnetCatch::UpdateUse(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)
{
	MoveObject(_obj);
	startDirecPos_ = _playerPos;
	goalDirecPos_= _obj.lock()->GetPos();
}

void MagnetCatch::UpdateDirection(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos)
{
	//初手のゴール位置設定
	if (!isSetGoalPos_) {
		if (_obj.expired()) {
			VECTOR woldPos = ConvScreenPosToWorldPos(AbilityManager::RETICLE_POS);
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
			auto& camera=SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::LOCKON);
			camera.SetLockPos(goalDirecPos_, false);
		}
	}

	//演出更新
	startDirecPos_ = _playerPos;
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
}

void MagnetCatch::ResetAbility(void)
{
	startDirecPos_ = Utility::VECTOR_ZERO;
	goalDirecPos_ = Utility::VECTOR_ZERO;
	nowPos_ = Utility::VECTOR_ZERO;
	isSetGoalPos_ = false;
	direcStep_ = 0.0f;
}

void MagnetCatch::MoveObject(std::weak_ptr<GimmickObjBase> _obj)
{
	InputManager& ins = InputManager::GetInstance();
	VECTOR movedPos = _obj.lock()->GetPos();

	if (ins.IsPressed("subUp"))
	{
		movedPos.y += ACC_MOVE;
	}
	if (ins.IsPressed("subDown"))
	{
		movedPos.y -= ACC_MOVE;
	}
	if (ins.IsPressed("subLeft"))
	{
		movedPos.x -= ACC_MOVE;
	}
	if (ins.IsPressed("subRight"))
	{
		movedPos.x += ACC_MOVE;
	}

	_obj.lock()->SetPos(movedPos);
}
