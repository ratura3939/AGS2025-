#include"../../Utility/Utility.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include "CharacterBase.h"

void CharacterBase::Init(void)
{
	modelId_ = -1;

	scl_ = Utility::VECTOR_ONE;
	rot_ = { 0.0f,0.0f,-1.0f };;
	pos_ = Utility::VECTOR_ZERO;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotLocal_ = Quaternion();
	startQua_ = Quaternion();
	goalQua_ = Quaternion();
	stepRotation_ = 0.0f;

	SetPram();
	//モデル各種最終設定用に更新をかける
	UpdateRotQuat();
}

void CharacterBase::SetPram(void)
{
	//キャラ固有の初期位置
}

void CharacterBase::Draw(void)
{
	//描画処理
	DrawSphere3D(pos_, 30, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(modelId_);
}

void CharacterBase::Release(void)
{
}

void CharacterBase::UpdateRotQuat(void)
{
	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1)
	{
		MV1SetMatrix(modelId_, mat);
	}
}

void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQua_);
	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotation_ = TIME_ROT;
	}
	startQua_ = quaRot_;
	goalQua_ = axis;
}

void CharacterBase::Rotation(void)
{
	stepRotation_ += SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	quaRot_ = Quaternion::Slerp(
		startQua_, goalQua_, stepRotation_);
}

VECTOR CharacterBase::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

VECTOR CharacterBase::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

VECTOR CharacterBase::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

VECTOR CharacterBase::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

VECTOR CharacterBase::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

VECTOR CharacterBase::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

VECTOR CharacterBase::GetDir(const VECTOR& _vec) const
{
	return quaRot_.PosAxis(_vec);
}

const VECTOR CharacterBase::GetPos(void) const
{
	return pos_;
}

const Quaternion CharacterBase::GetQua(void) const
{
	return quaRot_;
}

void CharacterBase::DrawDebug(void)
{
}
