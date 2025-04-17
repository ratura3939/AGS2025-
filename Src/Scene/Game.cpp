#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include "Game.h"

Game::Game(void)
{
	//確認用これはAGS2025夏用のプロジェクトです
	//確認用Part２
}

Game::~Game(void)
{
}

void Game::Init(void)
{
	//プレイヤーの生成
	player_ = std::make_unique<PlayerManager>();
	player_->Init();

	Camera& camera = SceneManager::GetInstance().GetCamera();
	//カメラの初期設定
	camera.ChangeMode(Camera::MODE::FOLLOW);
	//カメラにプレイヤーの設定する
	camera.SetFollow(player_->GetPos(), player_->GetQua());
}

void Game::Update(void)
{
	//プレイヤー
	player_->Update();
	//カメラにプレイヤーの設定する
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.SetFollow(player_->GetPos(),player_->GetQua());
	camera.SetTargetPos(player_->GetFocusPoint());
}

void Game::Draw(void)
{
	//DrawString(0, 0, "GameScene", 0xffffff, true);
	//プレイヤー
	player_->Draw();

	DrawDebug();
}

void Game::Release(void)
{
	//プレイヤー
	player_->Release();
}

void Game::DrawDebug(void)
{
	SceneManager::GetInstance().GetCamera().DrawDebug();
	player_->DrawDebug();
}
