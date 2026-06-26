#pragma once

class PixelRenderer;
class Game;

class CutSceneBase
{
public:
	CutSceneBase(void);
	virtual ~CutSceneBase(void);

	void Init(void);	//初期化
	bool Update(void);	//更新
	void Draw(void);	//描画
	void Release(void);	//解放

protected:
	virtual void DoInit(void) = 0;		//派生先各種初期化処理
	virtual bool DoUpdate(void) = 0;	//派生先各種更新処理
	virtual void DoDraw(void) = 0;		//派生先各種描画処理
	virtual void DoRelease(void) = 0;	//派生先各種解放処理

	void DrawPostEffect(PixelRenderer& _renderer);	//ポストエフェクトの描画
	virtual void FinishDirection(void) = 0;			//演出終了時処理

	int directionCounter_;	//演出に関するカウンター
	int postEffectScreen_;	//ポストエフェクトをかける用のスクリーン
};

