#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/Decoration/SoundManager.h"
#include "GateGimmickObjs.h"

GateGimmickObjs::GateGimmickObjs(const VECTOR& _switchPos, const VECTOR& _gatePos, const Quaternion& _gateQua)
{
	gate_ = std::make_unique<Gate>(_gatePos,_gateQua);
	switchObj_ = std::make_unique<SwitchObj>(_switchPos);

	SoundManager& sndM = SoundManager::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	sndM.Add(SoundManager::TYPE::SE, "MoveFence", rsM.Load(ResourceManager::SRC::FENCE_SE).handleId_);
	sndM.Add(SoundManager::TYPE::SE, "PressSwitch", rsM.Load(ResourceManager::SRC::SWITCH_SE).handleId_);


	sndM.AdjustVolume("MoveFence", 70.0f);
}

GateGimmickObjs::~GateGimmickObjs(void)
{
}

void GateGimmickObjs::Draw(void)
{
	gate_->Draw();
	switchObj_->Draw();

	//gate_->DrawDebug();
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
		SoundManager& sndM = SoundManager::GetInstance();
		sndM.Play("PressSwitch");
		sndM.Play("MoveFence");

		Gate::GATE_STATE gateState = Gate::GATE_STATE::CLOSE;

		if (switchObj_->IsPressFinish()) {
			gateState = Gate::GATE_STATE::OPEN;;
		}

		gate_->SetState(gateState);
	}
}
