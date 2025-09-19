#include "GimmickObjBase.h"

void GimmickObjBase::UpdateRotQuat(void)
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

GimmickObjBase::GimmickObjBase(void)
{
}

GimmickObjBase::~GimmickObjBase(void)
{
}

void GimmickObjBase::Init(void)
{
}

void GimmickObjBase::Update(void)
{
}

void GimmickObjBase::Draw(void)
{
}

void GimmickObjBase::Release(void)
{
}
