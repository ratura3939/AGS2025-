#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Utility/Utility.h"
#include "AppearBossScene.h"

void AppearBossScene::Init(void)
{
}

void AppearBossScene::Update(void)
{
	//危険のポストエフェクト→画面揺れ→カメラ
	if ((this->*direcUpdate_)()) {
		//次の演出に
		direcState_ = static_cast<BOSS_DIRECTION>(static_cast<int>(direcState_) + 1);
		direcCnt_ = 0;

		//もし終了したら
		if (direcState_ == BOSS_DIRECTION::END) {
			//カメラの追従対象を戻す
			Camera& camera = SceneManager::GetInstance().GetCamera();
			camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
			camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
			camera.SetGoalFocusPos(player_->GetFocusPoint());				//注視点

			//ブラーをなくす
			ChangeActionDirec(ACTION_DIRECTION::NORMAL);

			//BGM流す
			SoundManager::GetInstance().Play("BossBgm");
			nowBgmStr_ = "BossBgm";
			switchBgm_ = false;

			//更新を通常に
			update_ = &Game::GameUpdate;
		}
		//画面揺れ
		else if (direcState_ == BOSS_DIRECTION::SHAKE_SCREEN) {
			//実行
			DoShake();
			//演出の更新を「画面揺れ」に
			direcUpdate_ = &Game::DirectionShakeScreen;
		}
		//カメラ移動
		else if (direcState_ == BOSS_DIRECTION::CAMERA_MOVE) {

			auto& camera = SceneManager::GetInstance().GetCamera();
			//ボスの生成
			enemy_->CreateBoss(player_->GetPos());
			camera.SetLockOnDistanceMin(LOCK_DISTANCE_MIN_BOSS);			//ロックオン最低距離

			//カメラを自動移動に設定
			camera.ChangeMode(Camera::MODE::AUTO_MOVE);

			//場所の設定(ボスの横ぐらい)
			auto bossPos = enemy_->GetPos(BOSS_IDX);
			camera.SetPos(VAdd(bossPos, cameraMoveStartPos_), bossPos);
			camera.SetGoalDirecPos(VAdd(bossPos, cameraMoveGoalPos_[direcCnt_]));

			//演出を「カメラ移動に変更
			direcUpdate_ = &Game::DirectionCameraMove;
		}
	}
}

void AppearBossScene::Draw(void)
{
}

void AppearBossScene::Release(void)
{
}

bool AppearBossScene::UpdatePostEffect(void)
{
	//WARNING更新
	UIManager2d::GetInstance().Update(WARNING_STR_IMG);

	//ポストエフェクト更新
	scanLineMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });	//横ライン移動用

	//演出カウンタ更新
	direcCnt_++;
	//一定時間過ぎたら
	if (direcCnt_ > WARNING_DIRECTION_TIME) {
		//演出終了
		SoundManager::GetInstance().Stop("WarningBgm");	//警告音止める
		ChangeActionDirec(ACTION_DIRECTION::NORMAL);		//ポストエフェクト終了
		return true;
	}
	//演出が続く
	return false;
}

bool AppearBossScene::UpdateShakeScreen(void)
{
	//カメラノーシェイク時
	if (stayCameraShake_) {
		//クールタイム増加
		cameraShakeCollTimeCnt_++;

		//一定時間経過後
		if (cameraShakeCollTimeCnt_ >= CAMERA_SHAKE_COOL_TIME) {
			//再度揺らす
			DoShake();
			stayCameraShake_ = false;
		}
		return false;
	}

	//カメラシェイク終了時
	if (SceneManager::GetInstance().GetCamera().IsFinishShake()) {
		//演出カウンタ増加
		direcCnt_++;

		//一定数行ったら
		if (direcCnt_ >= CAMERA_SHAKE_NUM) {
			//演出終了
			return true;
		}
		//クールタイム関係リセット
		cameraShakeCollTimeCnt_ = 0;
		stayCameraShake_ = true;
	}
	//演出が続く
	return false;
}

void AppearBossScene::DoShake(void)
{
	SoundManager::GetInstance().Play("Impact");	//効果音再生
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::SHAKE);	//揺らす
}

bool AppearBossScene::UpdateCameraMove(void)
{
	//アニメーションのみ更新
	enemy_->UpdateAnim();

	//カメラ演出用
	auto& camera = SceneManager::GetInstance().GetCamera();

	//ゴール位置についたら次のスタート位置へ
	auto cameraPos = camera.GetPos();
	if (Utility::MagnitudeF(VSub(camera.GetGoalPos(), cameraPos)) <= ALLOWABLE_DISTANCE) {
		//演出カウンタ増加
		direcCnt_++;

		//移動演出回数の上限に到達していたら
		if (direcCnt_ >= CAMERA_DIRECTION_NUM) {
			//演出終了
			return true;
		}
		else {
			//次の目標地点への設定
			camera.SetGoalDirecPos(VAdd(enemy_->GetPos(BOSS_IDX), cameraMoveGoalPos_[direcCnt_]));

			//二回目の移動はボスの「叫び」も入れる
			enemy_->BossShout();
			//「叫び」演出用のブラーへ
			ChangeActionDirec(ACTION_DIRECTION::BLUR);
		}
	}
	//演出が続く
	return false;
}

void AppearBossScene::DrawScanLine(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	SetDrawScreen(scanLineScreen_);

	// 画面を初期化
	//ClearDrawScreen();

	DrawGraph(0, 0, mainScreen, false);
	scanLineRender_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, scanLineScreen_, false);
	//ポストエフェクトの上から鮮明な文字を出す
	UIManager2d::GetInstance().Draw(WARNING_STR_IMG);
}

void AppearBossScene::DrawBlur(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	blurMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });

	SetDrawScreen(blurScreen_);

	// 画面を初期化
	//ClearDrawScreen();

	DrawGraph(0, 0, mainScreen, false);
	blurRender_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, blurScreen_, false);
}
