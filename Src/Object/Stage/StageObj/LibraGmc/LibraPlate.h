#pragma once
#include "../NoneAfectAbilityObjBase.h"
class LibraPlate :
    public NoneAfectAbilityObjBase
{
public:
    enum class LIBRA_PLATE_STATE {
       STAY
        ,UP
        ,DOWN
    };

    LibraPlate(const VECTOR& _pos, const Quaternion& _qua,const float _offset);
    ~LibraPlate(void)override;

    void HitCollider(std::weak_ptr<Collider> _col)override;

    void SetState(const LIBRA_PLATE_STATE& _state);

	const float GetCurrentLoadWeight(void)const { return currentLoadWeight_; }
	const float GetMoveOffset(void)const { return moveOffset_; }

private:
    void SetModel(void)override;
    void UpdateNomal(void)override;

	void ClampMoveOffset(void); //移動オフセットの制限

	LIBRA_PLATE_STATE state_;   //現在の状態
	float currentLoadWeight_;   //現在の載っている重さ
	float moveOffset_;		    //移動オフセット
};

