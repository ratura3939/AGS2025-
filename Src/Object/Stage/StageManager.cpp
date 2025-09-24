#include"StageObj/GimmickTestObj.h"
#include"StageObj/TestObj2.h"
#include"StageObj/Stage.h"
#include "StageManager.h"

void StageManager::Init(void)
{
	gmkObjs_.push_back(std::make_unique<GimmickTestObj>());
	gmkObjs_.push_back(std::make_unique<TestObj2>());
	gmkObjs_.push_back(std::make_unique<Stage>());

	for (auto& obj : gmkObjs_) {
		obj->Init();
	}
}

void StageManager::Update(void)
{
	for (auto& obj : gmkObjs_) {
		obj->Update();
	}
}

void StageManager::Draw(void)
{
	for (auto& obj : gmkObjs_) {
		obj->Draw();
	}
}

void StageManager::SetAbility(const FLOAT4 _color)
{
	for (auto& obj : gmkObjs_) {
		obj->SetObjectRenderColor(_color);
	}
}
