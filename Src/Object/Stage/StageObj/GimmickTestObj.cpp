#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include "GimmickTestObj.h"


void GimmickTestObj::SetPram(void)
{
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL).handleId_;
	pos_ = { 200.0f,100.0f,500.0f };
	scl_ = { 1.0f,1.0f ,1.0f };

	//shaderê›íË
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(objColor_);
}
