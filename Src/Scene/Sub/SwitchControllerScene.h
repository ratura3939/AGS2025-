#pragma once
#include "../SceneBase.h"
class SwitchControllerScene :
    public SceneBase
{
public:
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	int switchImg_;
};

