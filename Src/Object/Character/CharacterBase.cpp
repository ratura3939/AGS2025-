#include"../../Utility/Utility.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"../Common/Collider.h"
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

	centerPos_ = Utility::VECTOR_ZERO;
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::UpdateAnimOnly(void)
{
	animController_->Update();
}

void CharacterBase::Draw(void)
{
	//•`‰æˆ—
	//DrawSphere3D(pos_, 30, 10, 0xff0000, 0xff0000, false);
	MV1DrawModel(modelId_);
	DrawUI();
}

void CharacterBase::Release(void)
{
	
}

void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetAngle();

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


const VECTOR& CharacterBase::GetHeight(void) const
{
	return headPos_;
}

const float CharacterBase::GetCollisionRadius(void) const
{
	return collider_->GetGeometry().GetRadius();
}

void CharacterBase::SetPrePos(void)
{
	pos_ = prePos_;
}

const Quaternion& CharacterBase::GetQua(void) const
{
	return characterRotY_;
}

void CharacterBase::SetQua(const Quaternion _qua)
{
	characterRotY_ = _qua;
}

std::weak_ptr<Collider> CharacterBase::GetAttackCollider(void)
{
	return atkCollider_;
}

const bool CharacterBase::IsAlive(void) const
{
	return hp_ > 0;
}

void CharacterBase::Deth(void)
{
	hp_ = 0;
}

void CharacterBase::DrawDebug(void)
{
}

void CharacterBase::DrawCupcel(void)
{
	// ã‚Ì‹…‘Ì
	VECTOR pos1 = GetHeight();
	DrawSphere3D(pos1, CHARACTER_RADIUS, 5, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = GetPos();
	DrawSphere3D(pos2, CHARACTER_RADIUS, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ğŒq‚®ü(X+)
	dir = GetRight();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(X-)
	dir = GetLeft();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z+)
	dir = GetForward();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z-)
	dir = GetBack();
	s = VAdd(pos1, VScale(dir, CHARACTER_RADIUS));
	e = VAdd(pos2, VScale(dir, CHARACTER_RADIUS));
	DrawLine3D(s, e, COLOR);

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(VAdd(GetPos(),VScale(CHARACTER_HEIGHT,2.0f)), 5.0f, 10, COLOR, COLOR, true);
}
