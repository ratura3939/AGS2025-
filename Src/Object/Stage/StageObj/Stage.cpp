#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Renderer/ModelRenderer.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Draw(void)
{
	//render_->Draw();
	MV1DrawModel(modelId_);
}

void Stage::SetPram(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::STAGE_MDL).handleId_;
	pos_ = { 0.0f,-50.0f,0.0f };
	scl_ = { 100.0f,100.0f,100.0f };

	//shaderê›íË
	material_ = std::make_unique<ModelMaterial>("StdModel.cso", 0, "StdModel.cso", 1);
	objColor_ = { 1.0f,0.0f,0.0f };
	material_->AddConstBufPS(objColor_);
}
