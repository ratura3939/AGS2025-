#include<DxLib.h>
#include"StageObj/TestObj.h"
#include"StageObj/TestObj2.h"
#include"StageObj/GateGmc/GateGimmickObjs.h"
#include"StageObj/GateGmc/Gate.h"
#include"StageObj/Stage.h"
#include "StageManager.h"

namespace {
	const int STAEGE_OBJ_NUM = 0;	//地形が格納されているインデックス
	const VECTOR GATE_SWITCH_POS = { 500.0f,0.0f,0.0f };
	const VECTOR GATE_POS = { 1200.0f,1200.0f,0.0f };
}

void StageManager::Init(void)
{
	gmkObjs_.push_back(std::make_shared<Stage>());
	gmkObjs_.push_back(std::make_shared<GateGimmickObjs>(GATE_SWITCH_POS, GATE_POS, Quaternion()));
	gmkObjs_.push_back(std::make_shared<TestObj>());
	gmkObjs_.push_back(std::make_shared<TestObj2>());
	

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

void StageManager::SetAbilityColor(const FLOAT4& _color)
{
	for (auto& obj : gmkObjs_) {
		obj->SetObjectRenderColor(_color);
	}
}

std::vector<std::weak_ptr<GimmickObjBase>> StageManager::GetAffectAbilityObjectes(void) const
{
	std::vector<std::weak_ptr<GimmickObjBase>> ret;
	for (auto& obj : gmkObjs_) {
		if (obj->IsAffectAbility()) {
			ret.push_back(obj);
		}
	}
	return ret;
}