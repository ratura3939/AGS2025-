#include "GateGimmickObjs.h"

GateGimmickObjs::GateGimmickObjs(const VECTOR& _switchPos, const VECTOR& _gatePos, const Quaternion& _gateQua)
{
	gate_ = std::make_unique<Gate>(_gatePos,_gateQua);
	switchObj_ = std::make_unique<SwitchObj>(_switchPos);
}

GateGimmickObjs::~GateGimmickObjs(void)
{
}

void GateGimmickObjs::Draw(void)
{
	gate_->Draw();
	switchObj_->Draw();
}

void GateGimmickObjs::HitCollider(std::weak_ptr<Collider> _col)
{
	//オブジェクトの紐づけクラスなので何もしない
}

void GateGimmickObjs::SetObjectRenderColor(const FLOAT4& _color)
{
	gate_->SetObjectRenderColor(_color);
	switchObj_->SetObjectRenderColor(_color);
}

void GateGimmickObjs::SetParam(void)
{
	gate_->Init();
	switchObj_->Init();
}

void GateGimmickObjs::UpdateNomal(void)
{
	gate_->Update();
	switchObj_->Update();

	//切り替えが行われていた時
	if (switchObj_->IsChangePressedTrg()) {

		Gate::GATE_STATE gateState = Gate::GATE_STATE::CLOSE;

		if (switchObj_->IsPressFinish()) {
			gateState = Gate::GATE_STATE::OPEN;;
		}

		gate_->SetState(gateState);
	}
}
