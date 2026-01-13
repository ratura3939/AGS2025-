#pragma once
#include "../GimmickObjBase.h"
class LibraGimmickObjs :
    public GimmickObjBase
{
public:

    //メモ：オフセットを追記するように
    LibraGimmickObjs(const VECTOR& _firstLibraPos, const VECTOR& _secondLibraPos, const Quaternion& _gateQua);
    ~LibraGimmickObjs(void)override;

    void Draw(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

    void SetObjectRenderColor(const FLOAT4& _color)override;

private:
    void SetParam(void)override;
    void UpdateNomal(void)override;
};