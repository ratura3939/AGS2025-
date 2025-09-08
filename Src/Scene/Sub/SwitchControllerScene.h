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

	int switchAfterCnt_;
	bool switchTrg_;
	bool isSwBtn_;	//êÿÇËë÷Ç¶É{É^ÉìÇ…Ç¢ÇÈÇ©Ç«Ç§Ç©
};

