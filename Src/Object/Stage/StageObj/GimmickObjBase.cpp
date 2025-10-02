#include "GimmickObjBase.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"



GimmickObjBase::GimmickObjBase(void)
{
	objColor_ = { 0.0f,0.0f,0.0f,1.0f };
}

GimmickObjBase::~GimmickObjBase(void)
{
}

void GimmickObjBase::Init(void)
{
	//各オブジェクトの設定
	SetPram();

	MV1SetPosition(modelId_, pos_);
	MV1SetScale(modelId_, scl_);

	render_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	MV1SetupCollInfo(modelId_);
}

void GimmickObjBase::Update(void)
{
	UpdateRotQuat();
	MV1SetupCollInfo(modelId_);
	material_->SetConstBufPS(0, objColor_);
}

void GimmickObjBase::Draw(void)
{
	//MV1DrawModel(modelId_);
	render_->Draw();
}

void GimmickObjBase::Release(void)
{
}

const bool GimmickObjBase::IsHitCameraRay(const VECTOR _start, const VECTOR _end)const
{
	return MV1CollCheck_Line(modelId_,-1,_start,_end).HitFlag;
}

void GimmickObjBase::SetObjectRenderColor(const FLOAT4 _color)
{
	objColor_ = _color;
}

void GimmickObjBase::SetPos(const VECTOR _pos)
{
	pos_ = _pos;
}

VECTOR GimmickObjBase::GetPos(void)
{
	return pos_;
}

const bool GimmickObjBase::IsAffectAbility(void) const
{
	return isAffectAbilyty_;
}

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