#include"../../Manager/Generic/ResourceManager.h"
#include "MagnetFallLine.h"

namespace {
	const float SIZE_Y_MAX = 20.0f; //Y方向の最大サイズ
}

MagnetFallLine::MagnetFallLine(void)
	: ActorBase()
	, material_(nullptr)
	, renderer_(nullptr)
	, modelInitSizeY_(0.0f)
	, nearFallPoint_(0.0f)
	, preNearFallPoint_(0.0f)
{
	isActiveGravity_ = false;
}

MagnetFallLine::~MagnetFallLine(void)
{
}

void MagnetFallLine::Draw(void)
{
}

void MagnetFallLine::Release(void)
{
}

void MagnetFallLine::HitCollider(std::weak_ptr<Collider> _col)
{
	
}

void MagnetFallLine::DoInit(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();

	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::MAGNET_LINE_MDL);
	material_ = std::make_unique<ModelMaterial>("MagnetLineVS.cso", 0,"MagnetLinePS.cso", 0);
	//追加テクスチャ挿入
	material_->SetTextureBuf(ModelMaterial::SUB_TEX_1, resM.Load(ResourceManager::SRC::LOCKON_IMG).handleId_);

	modelInitSizeY_ = MV1GetMeshMaxPosition(modelId_, 0).y - MV1GetMeshMinPosition(modelId_, 0).y;
}

void MagnetFallLine::DoUpdate(void)
{
	preNearFallPoint_ = nearFallPoint_;
	//落下地点が深くなったか
	//nearFallPoint_ == 0.0fのとき当たり判定が行われていない　＞　つまり深くなっている
	bool isLergerFallDipth = nearFallPoint_ == 0.0f;

	//初期化
	nearFallPoint_ = 0.0f;

	if (isLergerFallDipth) {
		//とりあえず大きくして判定をとるようにする

	}
}

void MagnetFallLine::ChangeSizeYToFallPoint(void)
{
}
