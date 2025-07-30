#pragma once
#include "../SceneBase.h"

#include<vector>
#include<string>

class PauseScene :
    public SceneBase
{
public:

	enum class TYPE_STEP {
		NONE,
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

private:
	void UserInput(void);

public:
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	const VECTOR GetArrowDrawPos(void)const;

private:
	std::vector<std::string>drawBtnList_;	//描画されるボタンを上から順に
	int selectIdx_;	//カーソル位置
};

