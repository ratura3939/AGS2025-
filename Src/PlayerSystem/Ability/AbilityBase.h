#pragma once
#include<memory>

class GimmickObjBase;
class AbilityManager;

class AbilityBase
{
public:
	AbilityBase(AbilityManager& _mng);
	virtual ~AbilityBase(void);
	virtual void Update(std::weak_ptr<GimmickObjBase> _obj) = 0;
	virtual void Draw(void) = 0;
	virtual void ResetAbility(void) = 0;

protected:
	AbilityManager& manager_;
};

