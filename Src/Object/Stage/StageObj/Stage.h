#pragma once
#include"GimmickObjBase.h"
#include"../../Common/Collider.h"

class Stage:
	public GimmickObjBase
{
public:
	Stage(void);
	~Stage(void)override;

	void Draw(void)override;

	void HitCollider(const Collider::MASTER_TYPE& _hitType)override;

private:
	void SetParam(void)override;

	int noiseTex_;;
};

