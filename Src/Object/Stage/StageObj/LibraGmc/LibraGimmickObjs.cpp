#include "LibraGimmickObjs.h"

namespace {
	
}

LibraGimmickObjs::LibraGimmickObjs(const VECTOR& _firstLibraPos, const VECTOR& _secondLibraPos, const Quaternion& _gateQua)
{
	firstPlate_ = std::make_unique<LibraPlate>(_firstLibraPos, _gateQua,0.0f);
	secondPlate_ = std::make_unique<LibraPlate>(_secondLibraPos, _gateQua,0.0f);
}

LibraGimmickObjs::~LibraGimmickObjs(void)
{
}

void LibraGimmickObjs::Draw(void)
{
	firstPlate_->Draw();
	secondPlate_->Draw();
}

void LibraGimmickObjs::HitCollider(std::weak_ptr<Collider> _col)
{
	//オブジェクトの紐づけクラスなので何もしない
}

void LibraGimmickObjs::SetObjectRenderColor(const FLOAT4& _color)
{
	firstPlate_->SetObjectRenderColor(_color);
	secondPlate_->SetObjectRenderColor(_color);
}

void LibraGimmickObjs::SetParam(void)
{
	//プレート二つ初期化
	firstPlate_->Init();
	secondPlate_->Init();
}

void LibraGimmickObjs::UpdateNomal(void)
{
	//プレート移動量を値取得
	const float firstPlateMove = firstPlate_->GetMoveOffset();

	//積載量比較
	const float firstWeight = firstPlate_->GetCurrentLoadWeight();
	const float secondWeight = secondPlate_->GetCurrentLoadWeight();

	using PLATE_STATE = LibraPlate::LIBRA_PLATE_STATE;
	PLATE_STATE firstState = PLATE_STATE::STAY;
	PLATE_STATE secondState = PLATE_STATE::STAY;

	//重いほうをDOWN、軽いほうをUPに設定
	if (firstWeight > secondWeight)
	{
		firstState = PLATE_STATE::DOWN;
		secondState = PLATE_STATE::UP;
	}
	else if (firstWeight < secondWeight)
	{
		firstState = PLATE_STATE::UP;
		secondState = PLATE_STATE::DOWN;
	}
	else {
		//同じ重さなら釣り合うように

		//一つ目が上がっていたら下げる
		if (firstPlateMove > 0.0f) {
			firstState = PLATE_STATE::DOWN;
			secondState = PLATE_STATE::UP;
		}
		else if(firstPlateMove < 0.0f) {
			firstState = PLATE_STATE::UP;
			secondState = PLATE_STATE::DOWN;
		}
	}

	//設定
	firstPlate_->SetState(firstState);
	secondPlate_->SetState(secondState);

	//天秤の動作処理
	firstPlate_->Update();
	secondPlate_->Update();

}
