#pragma once
#include "../GimmickObjBase.h"
#include"Gate.h"
#include"SwitchObj.h"

//SwitchオブジェクトとGateオブジェクトの紐づけクラス
//ステージマネージャに持たせたいため、GimmickObjBaseを継承

class GateGimmickObjs :
    public GimmickObjBase
{
public:
    GateGimmickObjs(const VECTOR& _switchPos,const VECTOR& _gatePos,const Quaternion& _gateQua);
    ~GateGimmickObjs(void)override;

    void Draw(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

    void SetObjectRenderColor(const FLOAT4& _color)override;

private:
    void SetParam(void)override;
    void UpdateNomal(void)override;

	std::unique_ptr<Gate> gate_;
	std::unique_ptr<SwitchObj> switchObj_;
};

