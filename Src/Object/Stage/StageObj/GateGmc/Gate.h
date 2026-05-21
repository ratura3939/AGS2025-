#pragma once
#include "../NoneAfectAbilityObjBase.h"

#include"Fence.h"

class Gate :
    public NoneAfectAbilityObjBase
{
public:
    enum class GATE_STATE {
        NORMAL
        ,OPEN
        ,CLOSE
    };

    Gate(const VECTOR& _pos, const Quaternion& _qua);
    ~Gate(void)override;

    void Draw(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

    void SetObjectRenderColor(const FLOAT4& _color)override;

    void SetState(const GATE_STATE& _state);

private:
    void SetModel(void)override;
    void UpdateNomal(void)override;

    std::unique_ptr<Fence> fence_;
    GATE_STATE state_;
};

