#include"../../Utility/Utility.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
	: modelId_(-1)
	, pos_(Utility::VECTOR_ZERO)
	, scl_(Utility::VECTOR_ONE)
	, rot_(Utility::VECTOR_ZERO)
	, matScl_(MGetIdent())
	, matRot_(MGetIdent())
	, matPos_(MGetIdent())
	, quaRot_(Quaternion::Euler(Utility::VECTOR_ZERO))
	, quaRotOrigin_(Quaternion::Euler(Utility::VECTOR_ZERO))
	, quaRotLocal_(Quaternion::Euler(Utility::VECTOR_ZERO))
	, speciesName_("")
	, power_(0.0f)
	, gravity_(Utility::VECTOR_ZERO)
	, isActiveGravity_(true)
	, prevPos_(Utility::VECTOR_ZERO)
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
	prevPos_ = pos_;
	//派生クラスの更新処理
	DoUpdate();
	//共通処理
	UpdateGravity();
	UpdateRotQuat();

	//外部影響のリセット
	externalVec_ = Utility::VECTOR_ZERO;
}

void ActorBase::SetPos(const VECTOR& _pos)
{
	pos_ = _pos;
}

void ActorBase::SetPrevPos(void)
{
	pos_ = prevPos_;
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

void ActorBase::AddExternalVec(const VECTOR& _vec)
{
	externalVec_ = VAdd(externalVec_, _vec);
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

void ActorBase::UpdateGravity(void)
{
	if (isActiveGravity_) {
		//重力処理
		gravity_.y += GRAVITY_POW;
		pos_ = VAdd(pos_, gravity_);
	}
}
