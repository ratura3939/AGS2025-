#include<DxLib.h>
#include"StageObj/TestObj.h"
#include"StageObj/TestObj2.h"
#include"StageObj/Board.h"
#include"StageObj/GateGmc/GateGimmickObjs.h"
#include"StageObj/GateGmc/Gate.h"
#include"StageObj/RotationGmc/SpinDisc.h"
#include"StageObj/StaticParts/Cross.h"
#include"StageObj/StaticParts/Stairs.h"
#include"StageObj/StaticParts/Stage.h"
#include"StageObj/StaticParts/StageWall.h"
#include"StageObj/StaticParts/InsideWall.h"
#include"StageObj/StaticParts/Block.h"
#include "StageManager.h"

namespace {
	const int STAEGE_OBJ_NUM = 0;	//地形が格納されているインデックス
	const VECTOR NOMAL_SIZE = { 1.0f,1.0f,1.0f };

	const VECTOR GATE_SWITCH_POS = { 500.0f,0.0f,0.0f };
	const VECTOR GATE_POS = { 1200.0f,1200.0f,0.0f };
	const VECTOR TABLE_POS = { -7000,300.0f,-2900.0f };
	const VECTOR TABLE_POS_2 = { -5900,300.0f,-1600.0f };
	const VECTOR TABLE_POS_3 = { -6800,300.0f,-200.0f };
	const VECTOR CROSS_POS = { -3000,300.0f,0.0f };
	const VECTOR SRAIRS_POS = { 6800.0f,0.0f,4600.0f };

	//ブロック
	const VECTOR BLOCK_SIZE = { 12.0f,1.5f,1.7f };
	const VECTOR BLOCK_POS = { 6800.0f,0.0f,4000.0f };
	const VECTOR BLOCK_POS_2 = { 6800.0f,0.0f,1900.0f };
	const VECTOR BLOCK_POS_3 = { 6800.0f,0.0f,0.0f };
	//ボード
	const VECTOR BOARD_POS = { 6800.0f,600.0f,5000.0f };
	const VECTOR BOARD_SIZE = { 1.0f,1.0f,5.0f };
}

void StageManager::Init(void)
{
	gmkObjs_.push_back(std::make_shared<Stage>());
	gmkObjs_.push_back(std::make_shared<StageWall>());
	gmkObjs_.push_back(std::make_shared<InsideWall>());
	gmkObjs_.push_back(std::make_shared<GateGimmickObjs>(GATE_SWITCH_POS, GATE_POS, Quaternion()));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_2));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_3));
	//gmkObjs_.push_back(std::make_shared<Cross>(CROSS_POS));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS, 180.0f));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS, BLOCK_SIZE));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_2, BLOCK_SIZE));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_3, BLOCK_SIZE));
	gmkObjs_.push_back(std::make_shared<Board>(BOARD_POS, BOARD_SIZE));
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

void StageManager::DrawDebug(void)
{
	for (auto& obj : gmkObjs_) {
		obj->DrawDebug();
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