#include<DxLib.h>
#include"StageObj/TestObj.h"
#include"StageObj/TestObj2.h"
#include"StageObj/GateGmc/GateGimmickObjs.h"
#include"StageObj/GateGmc/Gate.h"
#include"StageObj/RotationGmc/SpinDisc.h"
#include"StageObj/Cross.h"
#include"StageObj/Stairs.h"
#include"StageObj/Stage.h"
#include "StageManager.h"

namespace {
	const int STAEGE_OBJ_NUM = 0;	//地形が格納されているインデックス
	const VECTOR GATE_SWITCH_POS = { 500.0f,0.0f,0.0f };
	const VECTOR GATE_POS = { 1200.0f,1200.0f,0.0f };
	const VECTOR TABLE_POS = { -700,300.0f,0.0f };
	const VECTOR TABLE_POS_2 = { -1800,300.0f,0.0f };
	const VECTOR TABLE_POS_3 = { -1200,300.0f,800.0f };
	const VECTOR CROSS_POS = { -3000,300.0f,0.0f };
	const VECTOR SRAIRS_POS = { -1200,50.0f,2000.0f };
}

void StageManager::Init(void)
{
	gmkObjs_.push_back(std::make_shared<Stage>());
	gmkObjs_.push_back(std::make_shared<GateGimmickObjs>(GATE_SWITCH_POS, GATE_POS, Quaternion()));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_2));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_3));
	/*gmkObjs_.push_back(std::make_shared<Cross>(CROSS_POS));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS));*/
	gmkObjs_.push_back(std::make_shared<TestObj>());
	

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