#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include "TestObj2.h"

TestObj2::TestObj2(void)
{
}

TestObj2::~TestObj2(void)
{
}

void TestObj2::SetParam(void)
{
	isAffectAbilyty_ = true;
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TEST_MDL_2).handleId_;
	pos_ = { -200.0f,100.0f,500.0f };
	scl_ = { 15.0f,15.0f ,15.0f };

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "StdModelPS.cso", 1);
	material_->AddConstBufPS(NOMAL_COLOR);

	//デバッグ
	isDrawScreenPosCircle_ = true;
	screenPosColor_ = 0xff00ff;
}
