#include "../../Common/Quaternion.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Utility/Utility.h"
#include "SkyDome.h"


SkyDome::SkyDome(void)
{
}

SkyDome::~SkyDome(void)
{

}

void SkyDome::Init(void)
{
	// モデル制御の基本情報
	modelID_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKY_DOME);
	MV1SetScale(modelID_, SCALES);
	pos_ = { 0.0f, 0.0f, 0.0f };
	/*transform_.quaRot = Quaternion::Euler(
		0.0f,
		Utility::Deg2RadF(180.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();*/

	//Zバッファを無効
	MV1SetUseZBuffer(modelID_, false);
	MV1SetWriteZBuffer(modelID_, false);
}

void SkyDome::Update(void)
{
	/*Quaternion yRot = Quaternion::AngleAxis(Utility::Deg2RadF(ROT_SPEED), Utility::AXIS_Y);
	transform_.quaRot = Quaternion::Mult(transform_.quaRot, yRot);
	transform_.Update();*/
}

void SkyDome::Draw(void)
{
	MV1DrawModel(modelID_);
}

void SkyDome::Release(void)
{
}
