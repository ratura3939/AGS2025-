#pragma once
#include "SceneBase.h"
class PouseScene :
    public SceneBase
{
public:

	// コンストラクタ
	PouseScene(void);

	// デストラクタ
	~PouseScene(void);

	void Init(void) override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
};

