#include<EffekseerForDXLib.h>
#include"../../Manager/GameSystem/PlayerManager.h"
#include"../../Manager/GameSystem/EnemyManager.h"
#include"../../Manager/GameSystem/AttackManager.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/EffectManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Object/Stage/Stage.h"
#include"../../Utility/Utility.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"../../Application.h"
#include "Game.h"

namespace {
	constexpr VECTOR CAMERA_START_1 = { 600.0f,200.0f,0.0f };	//カメラ演出開始位置
	constexpr VECTOR CAMERA_GOAL_1 = { 600.0f,1000.0f,0.0f };	//カメラ演出目標位置その①
	constexpr VECTOR CAMERA_GOAL_2 = { 0.0f,800.0f,600.0f };	//カメラ演出目標位置その②
	constexpr float ALLOWABLE_DISTANCE = 10.0f;		//カメラの移動完了判定をがば目にするために
	constexpr int BOSS_IDX = 0;		//ボスの配列番号(ボス単体のため必ず0)
}

Game::Game(void)
{
	nearEnemyNum_ = -1;
	preNearEnemyNum_ = -1;
	isSlowEffect_ = false;
	slowCnt_ = -1;
	nextBgmVol_ = 0;

	directionCnt_ = 0;
	directionStartPos_ = CAMERA_START_1;
	directionGoalPos_[0] = CAMERA_GOAL_1;
	directionGoalPos_[1] = CAMERA_GOAL_2;
	direcState_ = BOSS_DIRECTION::NONE;
	directionCollTimeCnt_ = 0;
	stayCameraShake_ = false;

	actionDirec_ = ACTION_DIRECTION::NOMAL;
}

Game::~Game(void)
{
	DeleteGraph(scanLineScreen_);
	DeleteGraph(blurScreen_);
}

void Game::Init(void)
{
	update_ = &Game::GameUpdate;

	//生成
	//プレイヤー
	player_ = std::make_unique<PlayerManager>(*this);
	player_->Init();

	//敵
	enemy_ = std::make_unique<EnemyManager>(*this);
	enemy_->Init();

	//攻撃
	atkMng_ = std::make_unique<AttackManager>();
	//攻撃の登録
	AttackDataInit();

	//判定
	collision_ = std::make_unique<CollisionManager>();

	stage_ = std::make_unique<Stage>(false);
	stage_->Init();

	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetFocusPos(player_->GetFocusPoint());				//注視点

	//音関係初期設定
	InitSound();
	//エフェクト関係初期化
	InitEffect();
	//シェーダー初期化
	InitShader();

	

	//「WARNING」画像
	ResourceManager& rsM = ResourceManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	uiM.Add(warningStr_, rsM.Load(ResourceManager::SRC::WARNING_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::FLASHING, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(warningStr_, VECTOR{static_cast<float>(Application::SCREEN_SIZE_X)/2.0f,static_cast<float>(Application::SCREEN_SIZE_Y) / 2.0f,0.0f });
	uiM.SetUIDirectionPram(warningStr_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, 10.0f, 255.0f, 0.0f);
}

void Game::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	//BGM
	sndM.Add(SoundManager::TYPE::BGM, "NomalBgm",
		rsM.Load(ResourceManager::SRC::GAME_BGM).handleId_);
	//バトルBGM
	sndM.Add(SoundManager::TYPE::BGM, "BattleBgm",
		rsM.Load(ResourceManager::SRC::BATTLE_BGM).handleId_);

	//バトルBGM
	sndM.Add(SoundManager::TYPE::BGM, "BossBgm",
		rsM.Load(ResourceManager::SRC::BOSS_BGM).handleId_);

	//警告音
	sndM.Add(SoundManager::TYPE::BGM, "WarningBgm",
		rsM.Load(ResourceManager::SRC::WARNING_BGM).handleId_);

	//初手は普通のBGM
	sndM.Play("NomalBgm");
	nowBgmStr_ = "NomalBgm";
	switchBgmStr_ = "BattleBgm";

	//SE
	//歩く
	sndM.Add(SoundManager::TYPE::SE, "Walk",
		rsM.Load(ResourceManager::SRC::WALK_SE).handleId_,20);
	sndM.AdjustVolume("Walk",60);

	//走る
	sndM.Add(SoundManager::TYPE::SE, "Dush",
		rsM.Load(ResourceManager::SRC::RUN_SE).handleId_,10);
	sndM.AdjustVolume("Dush", 60);

	//剣を振る
	sndM.Add(SoundManager::TYPE::SE, "SwingSword",
		rsM.Load(ResourceManager::SRC::SWING_SWORD_SE).handleId_);
	//ロックオン
	sndM.Add(SoundManager::TYPE::SE, "RockOn",
		rsM.Load(ResourceManager::SRC::LOCK_ON_SE).handleId_);
	//プレイヤーを発見
	sndM.Add(SoundManager::TYPE::SE, "FindPlayer",
		rsM.Load(ResourceManager::SRC::FIND_PLAYER_SE).handleId_);
	//ダメージ
	sndM.Add(SoundManager::TYPE::SE, "Damage",
		rsM.Load(ResourceManager::SRC::DAMAGE_SE).handleId_);

	//ボス足音
	sndM.Add(SoundManager::TYPE::SE, "Impact",
		rsM.Load(ResourceManager::SRC::BOSS_IMPACT_SE).handleId_);
	sndM.AdjustVolume("Impact", 60);

	//攻撃警告音
	sndM.Add(SoundManager::TYPE::SE, "Allert",
		rsM.Load(ResourceManager::SRC::ATK_ALLERT_SE).handleId_);

	//回避音
	sndM.Add(SoundManager::TYPE::SE, "Dodge",
		rsM.Load(ResourceManager::SRC::DODGE_SE).handleId_);
	sndM.AdjustVolume("Dodge", 45);

	//ジャスト回避音
	sndM.Add(SoundManager::TYPE::SE, "JustDodge",
		rsM.Load(ResourceManager::SRC::JUST_DODGE_SE).handleId_);
	sndM.AdjustVolume("JustDodge", 80);

}

void Game::InitEffect(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	EffectManager& efcM = EffectManager::GetInstance();

	//剣
	efcM.Add("Sword", rsM.Load(ResourceManager::SRC::SWORD_EFC).handleId_);
	//ダメージ
	efcM.Add("Damage", rsM.Load(ResourceManager::SRC::DAMAGE_EFC).handleId_);
}

void Game::InitShader(void)
{
	//ブラー
	//PS
	blurMaterial_ = std::make_unique<PixelMaterial>("Blur.cso", 3);
	//拡散光
	blurMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,0.0f });
	//時間
	blurMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	//画面大きさ
	blurMaterial_->AddConstBuf({ Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,0.0f,0.0f });

	blurRender_ = std::make_unique<PixelRenderer>(*blurMaterial_);
	blurRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// ポストエフェクト用スクリーン
	blurScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//ブラー
	//PS
	dodgeMaterial_ = std::make_unique<PixelMaterial>("JustDodgePS.cso", 3);
	//拡散光
	dodgeMaterial_->AddConstBuf({ 1.0f,1.0f,1.0f,1.0f });
	//時間
	dodgeMaterial_->AddConstBuf({ 0.0f,0.0f ,0.0f,0.0f });
	//画面X・Y・強さ・半径
	dodgeMaterial_->AddConstBuf({ Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y ,0.02f,0.4f });

	dodgeMaterial_->SetTextureBuf(11, ResourceManager::GetInstance().Load(ResourceManager::SRC::FOCUS_IMG).handleId_);

	dodgeRender_ = std::make_unique<PixelRenderer>(*dodgeMaterial_);

	dodgeRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// ポストエフェクト用スクリーン
	dodgeScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//走査線
	//PS
	scanLineMaterial_ = std::make_unique<PixelMaterial>("ScanLine.cso", 2);
	//拡散光
	scanLineMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,0.0f });
	//時間
	scanLineMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });

	scanLineRender_ = std::make_unique<PixelRenderer>(*scanLineMaterial_);
	scanLineRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// ポストエフェクト用スクリーン
	scanLineScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void Game::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();
	InputManager& inpM = InputManager::GetInstance();

	

#pragma region シーン遷移
	//プレイヤーが死んでいたら
	if (!player_->IsAlive()) {
		//BGM念のため両方停止
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}
	

	//ポーズシーン遷移
	if (inpM.IsTrigerrDown("pause")) {
		//シーン追加(一つ次へ)
		scM.PushSubScene();
	}
#pragma endregion

	//更新
	(this->*update_)();
}

void Game::GameUpdate(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();
	

	//敵がいなくなったら
	if (enemy_->GetEnemys().size() <= 0) {
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}


#pragma region 基礎アプデ
	player_->Update(*atkMng_);
	//敵はスローの効果を受ける
	if (isSlowEffect_) {
		//スロー時の更新(このカウンタはスローの影響を受けない)
		slowCnt_++;
		if (slowCnt_ >= LIMIT_SLOW) {
			EndSlow();
		}
	}
	//敵
	enemy_->Update(player_->GetPos(), *atkMng_);
	//攻撃
	atkMng_->Update();

	//判定処理/その中でスロー演出が入るかどうか
	if (collision_->Collision(player_->GetPlayer(), enemy_->GetEnemys(), atkMng_->GetActiveAttacks())) {
		StartSlow();
	}


#pragma endregion

#pragma region BGM


	//敵の状態(戦闘・それ以外)のトリガ
	if (enemy_->IsSwitchBattleOrNomalEnemyTrg()) {
		//もともと切り換え中だったら
		if (switchBgm_) {
			//強制終了処理
			FinishSwitchBgm();
		}

		//切り換え開始
		switchBgm_ = true;
		//切り替え後の再生
		sndM.Play(switchBgmStr_);
	}
	//BGM切り換え実行中
	if (switchBgm_) {
		//音量調整に加算
		nextBgmVol_ += BGM_VOL_ACC;
		sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);			//次のBGMは音量をあげる
		sndM.AdjustVolume(nowBgmStr_, (100 - nextBgmVol_));	//現在のBGMは音量を下げる

		//もしボリュームが100以上なら
		if (nextBgmVol_ >= 100) {
			nextBgmVol_ = 100;	//音量を100％に
			//終了処理
			FinishSwitchBgm();
		}
	}
#pragma endregion


	//TODO
	// カメラのロックオンの処理の最適化
	//ロックオン関係

	//下準備
	//対象の検索
	preNearEnemyNum_ = nearEnemyNum_;	//保存
	nearEnemyNum_ = DecideRockEnemy();	//新規検索

	//カメラ非ロックオン時
	if (camera.GetMode() != Camera::MODE::LOCKON) {
		//ロックオン対象が変わったとき
		if (preNearEnemyNum_ != nearEnemyNum_) {
			//更新処理
			enemy_->SetTargetEnemy(nearEnemyNum_);
		}
		//対象となる敵がいないとき
		if (nearEnemyNum_ < 0) {
			enemy_->NoTargetEnemy();
		}
	}
	else {
		//ロックオン中
		//押下終了時
		if (InputManager::GetInstance().IsTrigerrUp("rock")) {
			//各種状態の変化
			RockOff();
		}

		//対象となる敵がいなかったら
		if (nearEnemyNum_ < 0) {
			//各種状態の変化
			RockOff();
		}
	}

	//ロックオン処理
	//押下時
	if (InputManager::GetInstance().IsTrigerrDown("rock")) {
		//敵が存在するとき
		if (enemy_->GetEnemys().size() > 0) {
			//近くに敵がいるとき
			if (nearEnemyNum_ >= 0) {
				SoundManager::GetInstance().Play("RockOn");
				RockOn();
			}
		}
	}

#pragma region カメラ更新
	//カメラの設定
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象の更新

	Camera::MODE mode = camera.GetMode();
	//追従時
	if (mode == Camera::MODE::FOLLOW) {
		//camera.SetFocusPos(player_->GetFocusPoint());//注視点の更新
		camera.SetFocusPos(player_->GetPos());//注視点の更新
	}
	//ロックオン時
	else if (mode == Camera::MODE::LOCKON) {
		camera.SetRockPos(enemy_->GetPos(nearEnemyNum_));	//ロックオン対象の設定
	}
#pragma endregion
}

void Game::DirectionUpdate(void)
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
			camera.SetFocusPos(player_->GetFocusPoint());				//注視点

			//ブラーをなくす
			ChangeActionDirec(ACTION_DIRECTION::NOMAL);

			//BGM流す
			SoundManager::GetInstance().Play("BossBgm");
			nowBgmStr_ = "BossBgm";
			switchBgm_ = false;

			//更新を通常に
			update_ = &Game::GameUpdate;
		}
		//画面揺れ
		else if (direcState_ == BOSS_DIRECTION::SHAKE_SCREEN) {
			DoShake();
			direcUpdate_ = &Game::DirectionShakeScreen;
		}
		//カメラ移動
		else if (direcState_ == BOSS_DIRECTION::CAMERA_MOVE) {
			enemy_->CreateBoss();
			auto& camera = SceneManager::GetInstance().GetCamera();
			//カメラを自動移動に設定
			camera.ChangeMode(Camera::MODE::AUTO_MOVE);
			//場所の設定
			auto bossPos = enemy_->GetPos(BOSS_IDX);
			camera.SetPos(VAdd(bossPos,directionStartPos_), bossPos);
			camera.SetGoalPos(VAdd(bossPos, directionGoalPos_[directionCnt_]));
			direcUpdate_ = &Game::DirectionCameraMove;
		}

	}
}

bool Game::DirectionPostEffect(void)
{
	//WARNING更新
	UIManager2d::GetInstance().Update(warningStr_);
	//ポストエフェクト更新
	scanLineMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	direcCnt_++;
	if (direcCnt_ > WARNING_DIRECTION_TIME) {
		SoundManager::GetInstance().Stop("WarningBgm");	//警告音止める
		return true;
	}
	return false;
}

bool Game::DirectionShakeScreen(void)
{
	//カメラノーシェイク時
	if (stayCameraShake_) {
		directionCollTimeCnt_++;
		if (directionCollTimeCnt_ >= CAMERA_SHAKE_COOL_TIME) {
			DoShake();
			stayCameraShake_ = false;
		}
		return false;
	}


	//カメラシェイク終了時
	if (SceneManager::GetInstance().GetCamera().IsFinishShake()) {
		direcCnt_++;
		if (direcCnt_ >= CAMERA_SHAKE_NUM) {
			return true;
		}
		directionCollTimeCnt_ = 0;
		stayCameraShake_ = true;
	}
	return false;
}

void Game::DoShake(void)
{
	SoundManager::GetInstance().Play("Impact");
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::SHAKE);
}

bool Game::DirectionCameraMove(void)
{
	//アニメーションのみ更新
	enemy_->UpdateAnim();

	//カメラ演出用
	auto& camera = SceneManager::GetInstance().GetCamera();
	//ゴール位置についたら次のスタート位置へ
	auto cameraPos = camera.GetPos();
	if (Utility::MagnitudeF(VSub(camera.GetGoalPos(), cameraPos)) <= ALLOWABLE_DISTANCE) {
		directionCnt_++;
		//移動演出回数の上限に到達していたら
		if (directionCnt_ >= CAMERA_DIRECTION_NUM) {
			return true;
		}
		else {
			//次の目標地点への設定
			camera.SetGoalPos(VAdd(enemy_->GetPos(BOSS_IDX), directionGoalPos_[directionCnt_]));
			enemy_->BossShout();
			ChangeActionDirec(ACTION_DIRECTION::BLUR);
		}
	}
	return false;
}

void Game::Draw(void)
{
	stage_->Draw();
	enemy_->Draw();
	player_->Draw();

	//DrawDebug();

	if (direcState_ == BOSS_DIRECTION::POST_EFFECT) {
		DrawScanLine();
	}
	if (actionDirec_ == ACTION_DIRECTION::BLUR) {
		DrawBlur();
	}
	else if (actionDirec_ == ACTION_DIRECTION::JUST_DODGE) {
		DrawDodgeEffect();
	}
}

void Game::DrawScanLine(void)
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
	UIManager2d::GetInstance().Draw(warningStr_);
}

void Game::DrawBlur(void)
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

void Game::DrawDodgeEffect(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	dodgeMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });

	SetDrawScreen(dodgeScreen_);

	// 画面を初期化
	//ClearDrawScreen();

	DrawGraph(0, 0, mainScreen, false);
	dodgeRender_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, dodgeScreen_, false);
}

void Game::Release(void)
{
	player_->Release();
	enemy_->Release();
}

void Game::Reset(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 80);	//前シーンに戻るのでBGMの音量を復活
	//とりあえずメニューからの復帰時は追従に
	//メニュー開く直前に変える可能性大
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void Game::StartBossFaze(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	ChangeActionDirec(ACTION_DIRECTION::NOMAL);
	sndM.Stop("NomalBgm");	//今まで流していたものを停止
	sndM.Stop("BattleBgm");	//今まで流していたものを停止
	sndM.Play("WarningBgm");	//警告音流す
	direcState_ = BOSS_DIRECTION::POST_EFFECT;
	update_ = &Game::DirectionUpdate;
	direcUpdate_ = &Game::DirectionPostEffect;
}

void Game::ChangeActionDirec(const ACTION_DIRECTION _direc)
{
	actionDirec_ = _direc;
}

void Game::AttackDataInit(void)
{
	atkMng_->AddAttack(PlayerManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD,false, false, PlayerManager::ATTACK_TIME);
	atkMng_->AddAttack(EnemyManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, true,false, EnemyManager::ATTACK_TIME, EnemyManager::ATTACK_TIME_START, EnemyManager::ATTACK_TIME_END);
}

const int Game::DecideRockEnemy(void)
{
	return enemy_->GetNearEnemyNum(player_->GetPos());
}

void Game::FinishSwitchBgm(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	//切り換え終了
	switchBgm_ = false;
	sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);
	sndM.Stop(nowBgmStr_);	//今まで流していたものを停止
	//現在のBGM名と切り替え後のBGM名の切り換え
	//後々ボス個体の物も用意するのでそこで要調整
	auto ret = nowBgmStr_;
	nowBgmStr_ = switchBgmStr_;
	switchBgmStr_ = ret;
	//初期化
	nextBgmVol_ = 0;
}

void Game::RockOn(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.SetRockPos(enemy_->GetPos(nearEnemyNum_));	//ロックオン対象の設定
	player_->LockOn();
	enemy_->LokedOn(nearEnemyNum_);
	camera.ChangeMode(Camera::MODE::LOCKON);
}

void Game::RockOff(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();
	player_->LockOff();
	enemy_->NoTargetEnemy();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	//ゾーンを続かせないために
	ChangeActionDirec(ACTION_DIRECTION::NOMAL);
	EndSlow();
	//対象をキャンセルしたとみなし初期化する
	nearEnemyNum_ = -1;
}

void Game::StartSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	//スロー演出準備
	slowCnt_ = 0;
	ChangeActionDirec(ACTION_DIRECTION::JUST_DODGE);
	isSlowEffect_ = true;
	//更新スピードを50％に設定
	scM.SetUpdateSpeedRate_(SLOW_SPEED_PERCENT);
	//敵もそれに対応
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());
}

void Game::EndSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	isSlowEffect_ = false;
	ChangeActionDirec(ACTION_DIRECTION::NOMAL);
	//更新処理を100％にもどす
	scM.SetUpdateSpeedRate_(NOMAL_SPEED_PERCENT);
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent_());
}


void Game::DrawDebug(void)
{
	//SceneManager::GetInstance().GetCamera().DrawDebug();
	//if (isSlowEffect_) {
	//	DrawString(0, 140, "NOW_SLOW", 0xffffff);
	//}

	////player_->DrawDebug();
	//enemy_->DrawDebug();
	atkMng_->DrawDebug();

}
