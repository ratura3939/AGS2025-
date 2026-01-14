#include<DxLib.h>
#include"../../Utility/Utility.h"
#include"StageObj/TestObj.h"
#include"StageObj/TestObj2.h"
#include"StageObj/Board.h"
#include"StageObj/LibraGmc/LibraGimmickObjs.h"
#include"StageObj/GateGmc/GateGimmickObjs.h"
#include"StageObj/GateGmc/Gate.h"
#include"StageObj/RotationGmc/SpinDisc.h"
#include"StageObj/StaticParts/Cross.h"
#include"StageObj/StaticParts/Stairs.h"
#include"StageObj/StaticParts/Stage.h"
#include"StageObj/StaticParts/StageWall.h"
#include"StageObj/StaticParts/InsideWall.h"
#include"StageObj/StaticParts/Block.h"
#include"StageObj/StaticParts/WallSingle.h"
#include "StageManager.h"

namespace {
	const int STAEGE_OBJ_NUM = 0;	//地形が格納されているインデックス
	const VECTOR NOMAL_SIZE = { 1.0f,1.0f,1.0f };

	//ゲート
	const VECTOR GATE_SWITCH_POS = { -311.0f,0.0f,3500.0f };
	const VECTOR GATE_POS = { -311.0f,1800.0f,2923.0f };
	//回転テーブル
	const VECTOR TABLE_POS = { -7000,300.0f,-2900.0f };
	const VECTOR TABLE_POS_2 = { -6200,300.0f,-2000.0f };
	const VECTOR TABLE_POS_3 = { -6800,300.0f,-1100.0f };
	const VECTOR TABLE_POS_4 = { -6000,300.0f,-200.0f };
	const VECTOR TABLE_POS_5 = { -6930,300.0f,700.0f };
	//十字架
	const VECTOR CROSS_POS = { -3000,300.0f,0.0f };
	//階段
	const VECTOR SRAIRS_POS = { 3570.0f,0.0f,-6280.0f };
	const VECTOR SRAIRS_POS_2 = { 2380.0f,0.0f,-4800.0f };
	const VECTOR SRAIRS_POS_3 = { 380.0f,0.0f,-4800.0f };
	const VECTOR SRAIRS_POS_4 = { -4900.0f,0.0f,-3400.0f };
	const VECTOR SRAIRS_POS_5 = { -6740.0f,0.0f,-4649.0f };

	//ブロック
	const VECTOR BLOCK_SIZE = { 12.0f,1.5f,1.7f };
	const VECTOR BLOCK_SIZE_FOR_LIBRA = { 12.0f,2.0f,1.7f };
	const VECTOR BLOCK_POS = { 3000.0f,0.0f,-6280.0f };
	const VECTOR BLOCK_POS_2 = { 1000.0f,0.0f,-6280.0f };
	const VECTOR BLOCK_POS_3 = { -1000.0f,0.0f,-6280.0f };
	const VECTOR BLOCK_POS_4 = { -4800.0f,0.0f,-6261.0f };
	const VECTOR BLOCK_POS_5 = { -7045.0f,0.0f,-4033.0f };
	const VECTOR BLOCK_POS_6 = { -6284.0f,0.0f,1829.0f };
	//ボード
	const VECTOR BOARD_POS = { 2000.0f,500.0f,-6350.0f };
	const VECTOR BOARD_SIZE = { 1.0f,1.0f,5.0f };

	//壁
	const VECTOR WALL_POS = { 5200.0f,0.0f,-6400.0f };

	//天秤
	const VECTOR FIRST_LIBRA_POS = { -3900.0f,0.0f,-6570 };
	const VECTOR SECOND_LIBRA_POS = { -3900.0f,0.0f,-5400.0f };

	//回転量
	const float ROT_DEG_QUARTER = 90.0f;
	const float ROT_DEG_HALF = 180.0f;
}

void StageManager::Init(void)
{
	gmkObjs_.push_back(std::make_shared<Stage>());
	gmkObjs_.push_back(std::make_shared<StageWall>());
	gmkObjs_.push_back(std::make_shared<InsideWall>());
	gmkObjs_.push_back(std::make_shared<GateGimmickObjs>(GATE_SWITCH_POS, GATE_POS, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	//回転台
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_2));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_3));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_4));
	gmkObjs_.push_back(std::make_shared<SpinDisc>(TABLE_POS_5));
	//階段
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS, Quaternion::Euler(0.0f, Utility::Deg2RadF(-ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS_2, Quaternion()));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS_3, Quaternion()));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS_4, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Stairs>(SRAIRS_POS_5, Quaternion()));
	//ブロック
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS, BLOCK_SIZE, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_2, BLOCK_SIZE, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_3, BLOCK_SIZE, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_4, BLOCK_SIZE_FOR_LIBRA, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_5, BLOCK_SIZE, Quaternion()));
	gmkObjs_.push_back(std::make_shared<Block>(BLOCK_POS_6, BLOCK_SIZE, Quaternion()));
	//板
	gmkObjs_.push_back(std::make_shared<Board>(BOARD_POS, BOARD_SIZE, Quaternion::Euler(0.0f, Utility::Deg2RadF(ROT_DEG_QUARTER), 0.0f)));
	gmkObjs_.push_back(std::make_shared<LibraGimmickObjs>(FIRST_LIBRA_POS, SECOND_LIBRA_POS,Quaternion()));
	gmkObjs_.push_back(std::make_shared<WallSingle>(WALL_POS,Quaternion()));
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