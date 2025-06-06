#include "Stage.h"
#include"../../Utility/Utility.h"
#include"../../Manager/Generic/ResourceManager.h"

void Stage::Init(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::STAGE_MDL).handleId_;

	pos_ = Utility::VECTOR_ZERO;
	pos_.y = -50.0f;
	scl_ = { 100.0f,100.0f,100.0f };
	rot_ = Utility::VECTOR_ZERO;

	MV1SetPosition(modelId_,pos_);
}

void Stage::Update(void)
{
	// ‰ñ“]
	rot_ = quaRot_.ToEuler();
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
}
