#pragma once
#include "../SceneBase.h"
class PauseScene :
    public SceneBase
{
public:

	enum class TYPE_STEP {
		BACK_GAME,	//ゲームに戻る
		BACK_TITLE,	//ゲームやめる(タイトルに戻る)
		CONFIG,		//操作方法
		CHANGE_OPE,	//操作切り換え
	};

	// コンストラクタ
	PauseScene(void);

	// デストラクタ
	~PauseScene(void);

	void Init(void) override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

public:
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	bool isUpadate_;	//更新処理が行われたか
};

