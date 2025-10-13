#pragma once
#include<DxLib.h>
#include<memory>
#include"../../Common/Quaternion.h"

class GimmickObjBase;
class AbilityManager;

class AbilityBase
{
public:
	AbilityBase(AbilityManager& _mng);
	virtual ~AbilityBase(void);

	//影響を与えてからの更新
	virtual void UpdateUse(std::weak_ptr<GimmickObjBase> _obj, const VECTOR _playerPos, const Quaternion _playerQua) = 0;
	//使う際の演出用の更新
	virtual void UpdateDirection(std::weak_ptr<GimmickObjBase> _obj,const VECTOR _playerPos) = 0;
	virtual void Draw(void) = 0;
	virtual void ResetAbility(void) = 0;

protected:
	AbilityManager& manager_;
};

