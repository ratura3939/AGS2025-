#include"../../Utility/Utility.h"
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

void ActorBase::UpdateRotQuat(void)
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
	if (modelId_ != -1) {
		MV1SetMatrix(modelId_, mat);
	}
}