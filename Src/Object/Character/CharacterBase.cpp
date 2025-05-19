#include"../../Utility/Utility.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void)
{
	modelId_ = -1;
	pos_ = Utility::VECTOR_ZERO;
	scl_ = Utility::VECTOR_ZERO;
	rot_ = Utility::VECTOR_ZERO;
	hp_ = -1.0f;

	characterRotY_ = Quaternion();

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotOrigin_ = Quaternion();
	quaRotLocal_ = Quaternion();
	goalQua_ = Quaternion();
	stepRotation_ = 0.0f;
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::Update(void)
{
}

void CharacterBase::Update(const VECTOR _pPos)
{
}

void CharacterBase::Draw(void)
{
	//•`‰æˆ—
	DrawSphere3D(pos_, 30, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(modelId_);
}

const bool CharacterBase::Release(void)
{
	return true;
}

void CharacterBase::UpdateRotQuat(void)
{
	// ‘å‚«‚³
	matScl_ = MGetScale(scl_);

	// ‰ñ“]
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// ˆÊ’u
	matPos_ = MGetTranslate(pos_);

	// s—ñ‚Ì‡¬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// s—ñ‚ğƒ‚ƒfƒ‹‚É”»’è
	if (modelId_ != -1){
		MV1SetMatrix(modelId_, mat);
	}
}

void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetRot().ToEuler();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility::AXIS_Y);
	// Œ»İİ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ğæ‚é
	double angleDiff = Quaternion::Angle(axis, characterRotY_);
	// ‚µ‚«‚¢’l‚æ‚è‘å‚«‚©‚Á‚½‚ç
	if (angleDiff > THRESHOLD_ROT){
		//‰ñ“]‚µn‚ß‚Éİ’è
		stepRotation_ = 0.0f;
	}
	goalQua_ = axis;
}

void CharacterBase::Rotation(void)
{
	//‰ñ“]ŠÔ‘‰Á
	stepRotation_ +=PER_ROT;
	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	characterRotY_ = Quaternion::Slerp(
		characterRotY_, goalQua_, stepRotation_);

	//‰Šú‰»‚ğs‚¢V‚½‚È‰ñ“]—Ê‚ğİ’è‚·‚é
	quaRot_ = Quaternion();
	quaRot_ = quaRot_.Mult(characterRotY_);
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
	Quaternion retRot = Quaternion();
	//VECTOR‚È‚Ì‚ÅQuaternion‚É•Ï‰»
	retRot = retRot.Mult(characterRotY_);
	return retRot;
}

void CharacterBase::DrawDebug(void)
{
}
