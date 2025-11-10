#pragma once
#include "../GimmickObjBase.h"
class Fence :
    public GimmickObjBase
{
public:
    enum class MOVE_DIR {
        UP
        ,DOWN
    };

    Fence(const VECTOR& _pos, const Quaternion& _rot);
    ~Fence(void)override;

    void Draw(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;
    void MoveFnece(const MOVE_DIR _dir);
    const float GetMovedFenceDiff(void)const { return movedFenceDiff_; }

private:
    void SetParam(void)override;
    int movedFenceDiff_;    //çÚÇÃà⁄ìÆó 
};

