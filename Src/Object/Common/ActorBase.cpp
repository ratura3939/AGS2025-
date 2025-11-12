#include"../../Utility/Utility.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
{
}

ActorBase::~ActorBase(void)
{
}

void ActorBase::Init(void)
{
	DoInit();
	UpdateRotQuat();
	CollisionManager::GetInstance().AddCollider(collider_);	//ìñÇΩÇËîªíËìoò^
}

void ActorBase::Update(void)
{
	DoUpdate();
	UpdateRotQuat();
}

void ActorBase::SetPos(const VECTOR& _pos)
{
	pos_ = _pos;
}

const VECTOR& ActorBase::GetPos(void) const
{
	return pos_;
}

const VECTOR& ActorBase::GetForward(void) const
{
	return GetDir(Utility::DIR_F);
}

const VECTOR& ActorBase::GetBack(void) const
{
	return GetDir(Utility::DIR_B);
}

const VECTOR& ActorBase::GetRight(void) const
{
	return GetDir(Utility::DIR_R);
}

const VECTOR& ActorBase::GetLeft(void) const
{
	return GetDir(Utility::DIR_L);
}

const VECTOR& ActorBase::GetUp(void) const
{
	return GetDir(Utility::DIR_U);
}

const VECTOR& ActorBase::GetDown(void) const
{
	return GetDir(Utility::DIR_D);
}

const VECTOR& ActorBase::GetDir(const VECTOR& _vec) const
{
	return quaRot_.PosAxis(_vec);
}

const float ActorBase::GetPower(void) const
{
	return power_;
}

const std::string& ActorBase::GetSpeciesName(void) const
{
	return speciesName_;
}

void ActorBase::UpdateRotQuat(void)
{
	// ëÂÇ´Ç≥
	matScl_ = MGetScale(scl_);

	// âÒì]
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// à íu
	matPos_ = MGetTranslate(pos_);

	// çsóÒÇÃçáê¨
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// çsóÒÇÉÇÉfÉãÇ…îªíË
	if (modelId_ != -1) {
		MV1SetMatrix(modelId_, mat);
	}
}