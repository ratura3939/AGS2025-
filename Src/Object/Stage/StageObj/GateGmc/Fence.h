#pragma once
#include "../NoneAfectAbilityObjBase.h"
class Fence :
    public NoneAfectAbilityObjBase
{
public:
    enum class MOVE_DIR {
        UP
        ,DOWN
    };

    Fence(const VECTOR& _pos, const Quaternion& _rot);
    ~Fence(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;
    void MoveFnece(const MOVE_DIR _dir);
    const float GetMovedFenceDiff(void)const { return movedFenceDiff_; }

private:
    void SetModel(void)override;
    int movedFenceDiff_;    //ò‚ÌˆÚ“®—Ê
    VECTOR colliderPos_;    //ƒ‚ƒfƒ‹’†‰›‚É‚·‚é‚½‚ß‚ÌY²·•ª—pˆÊ’u
};

