#pragma once
#include<memory>
#include"CutSceneBase.h"

class PixelMaterial;
class PixelRenderer;
class PlayerManager;
class Game;

class DeathPlayer :
	public CutSceneBase
{
public:
	DeathPlayer(Game& _gameScene, PlayerManager& _player);
	~DeathPlayer(void)override;

private:
	void DoInit(void)override;
	bool DoUpdate(void)override;
	void DoDraw(void)override;
	void DoRelease(void)override;

	void FinishDirection(void)override;

	Game& gameScene_;		//ゲームシーン
	PlayerManager& player_;	//プレイヤー
	//走査線
	std::unique_ptr<PixelMaterial>irisOutMaterial_;
	std::unique_ptr<PixelRenderer>irisOutRender_;
};

