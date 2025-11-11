#pragma once
#include "../GimmickObjBase.h"
class SwitchObj :
    public GimmickObjBase
{
public:
    SwitchObj(const VECTOR& _pos);
    ~SwitchObj(void)override;

    void Draw(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

    const bool IsChangePressedTrg(void)const { return isPressFinish_ != isPrevPressFinish_; }
	const bool IsPressFinish(void)const { return isPressFinish_; }

private:
    void SetParam(void)override;
	void UpdateNomal(void)override;

    int pressCounter_;
    bool isPressFinish_;
    bool isPrevPressFinish_;
	float pressDepth_;
};

