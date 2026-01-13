#include "LibraGimmickObjs.h"

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
	//プレート移動量を絶対値取得
	//限界値に達していたら動かない（STAYに）→終了
	if (fabs(firstPlate_->GetMoveOffset()) >= 200.0f)
	{
		firstPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::STAY);
		secondPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::STAY);
		return;
	}

	//積載量比較
	const float firstWeight = firstPlate_->GetCurrentLoadWeight();
	const float secondWeight = secondPlate_->GetCurrentLoadWeight();

	//重いほうをDOWN、軽いほうをUPに設定
	if (firstWeight > secondWeight)
	{
		firstPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::DOWN);
		secondPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::UP);
	}
	else if (firstWeight < secondWeight)
	{
		firstPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::UP);
		secondPlate_->SetState(LibraPlate::LIBRA_PLATE_STATE::DOWN);
	}
}
