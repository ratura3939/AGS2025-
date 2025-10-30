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

	void HitCollider(std::weak_ptr<Collider> _col)override;

private:
	void SetParam(void)override;

	int noiseTex_;;
};

