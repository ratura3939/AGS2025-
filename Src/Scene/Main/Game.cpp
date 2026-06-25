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
#include "../../Scene/Main/GameOver.h"
#include "../../Scene/Main/GameClear.h"
#include "../../Scene/Sub/PauseScene.h"
#include "../../Scene/Main/GameDirection/AppearBoss.h"
#include "../../Scene/Main/GameDirection/DeathPlayer.h"
#include "../../Scene/Main/GameDirection/DeathBoss.h"
#include"../../Object/Stage/StageManager.h"
#include"../../Utility/Utility.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"../../Application.h"
#include "Game.h"

//ローカル定数
namespace {
	constexpr VECTOR CAMERA_START_1 = { 600.0f,200.0f,0.0f };	//カメラ演出開始位置
	constexpr VECTOR CAMERA_GOAL_1 = { 600.0f,1000.0f,0.0f };	//カメラ演出目標位置その①
	constexpr VECTOR CAMERA_GOAL_2 = { 0.0f,800.0f,600.0f };	//カメラ演出目標位置その②

	const int LIMIT_SLOW = 200;					//スロー演出時間
	const int BGM_VOL_MAX = 100;				//BGM音量最大値
	const int BGM_VOL_ACC = 1;					//BGM切り換えスピード
	const float NOMAL_SPEED_PERCENT = 100.0f;	//通常の割合
	const float SLOW_SPEED_PERCENT = 25.0f;		//スローの割合(通常時から半分の速度にする)
	
	const std::string MENU_BTN = "menuBtn";
	const float BTN_EX = 0.6f;
	const int BTN_DIFF_X = 300;
	const int BTN_DIFF_Y = 100;

	const float CAMERA_FOLLOW_DIFF_Y_ABILITY = 200.0f;	//能力使用時の注視点差分

	const float LOCK_DISTANCE_MIN_NOMAL = 500.0f;		//ロックオン時に最低限離れておく距離

	const int BGM_VOL = 80;	//BGMの音量
	const int WALK_SE_VOL = 60;	//歩くSEの音量
	const int DODGE_SE_VOL = 45;	//回避SEの音量
	const int JUST_DODGE_SE_VOL = 80;	//ジャスト回避SEの音量

	//ポストエフェクトバッファ数
	const int DODGE_NUM_BUFF_PS = 3;

	const float DODGE_EFFECT_RADIUS = 0.4f;
	const float DODGE_EFFECT_RATE = 0.02f;

	const int WALK_SE_INTERVAL = 20;	//歩くSEの再生間隔
	const int RUN_SE_INTERVAL = 10;		//走るSEの再生間隔
}

Game::Game(void):
	player_(nullptr)
	,enemy_(nullptr)
	,atkMng_(nullptr)
	,stage_(nullptr)
	,direction_(nullptr)
	,dodgeMaterial_(nullptr)
	,dodgeRender_(nullptr)
	,isSlowEffect_(false)
	,slowCnt_(-1)
	,nowBgmStr_("")
	,switchBgmStr_("")
	,nextBgmVol_(0)
	,switchBgm_(false)

{
	isDebug_ = false;
}

Game::~Game(void)
{
	DeleteGraph(dodgeScreen_);
}

void Game::Init(void)
{
	//リソース準備
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::GAME);

	//生成
	//ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	//攻撃
	atkMng_ = std::make_shared<AttackManager>();
	CollisionManager::GetInstance().SetAttackManager(atkMng_);

	//敵
	enemy_ = std::make_unique<EnemyManager>(*this, *atkMng_);
	
	//プレイヤー
	player_ = std::make_unique<PlayerManager>(*this, *enemy_, *atkMng_, *stage_);
	player_->Init();

	enemy_->Init(player_->GetPos());	//しぶしぶこの位置

	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetGoalFocusPos(player_->GetFocusPoint());			//注視点
	camera.SetLockOnDistanceMin(LOCK_DISTANCE_MIN_NOMAL);		//ロックオン最低距離

	//音関係初期設定
	InitSound();
	//エフェクト関係初期化
	InitEffect();
	//シェーダー初期化
	InitShader();

	auto& uiM = UIManager2d::GetInstance();

	//メニューボタン
	uiM.Add(MENU_BTN, rsM.Load(ResourceManager::SRC::MENU_BTN).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(MENU_BTN, VECTOR{ static_cast<float>(Application::SCREEN_SIZE_X - BTN_DIFF_X),static_cast<float>(Application::SCREEN_SIZE_Y - BTN_DIFF_Y),0.0f }, BTN_EX);
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

	//初手は普通のBGM
	sndM.Play("NomalBgm");
	nowBgmStr_ = "NomalBgm";
	switchBgmStr_ = "BattleBgm";

	//SE
	//歩く
	sndM.Add(SoundManager::TYPE::SE, "Walk",
		rsM.Load(ResourceManager::SRC::WALK_SE).handleId_, WALK_SE_INTERVAL);
	sndM.AdjustVolume("Walk", WALK_SE_VOL);

	//走る
	sndM.Add(SoundManager::TYPE::SE, "Dush",
		rsM.Load(ResourceManager::SRC::RUN_SE).handleId_, RUN_SE_INTERVAL);
	sndM.AdjustVolume("Dush", WALK_SE_VOL);

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
	sndM.AdjustVolume("Impact", WALK_SE_VOL);

	//攻撃警告音
	sndM.Add(SoundManager::TYPE::SE, "Allert",
		rsM.Load(ResourceManager::SRC::ATK_ALERT_SE).handleId_);

	//回避音
	sndM.Add(SoundManager::TYPE::SE, "Dodge",
		rsM.Load(ResourceManager::SRC::DODGE_SE).handleId_);
	sndM.AdjustVolume("Dodge", DODGE_SE_VOL);

	//ジャスト回避音
	sndM.Add(SoundManager::TYPE::SE, "JustDodge",
		rsM.Load(ResourceManager::SRC::JUST_DODGE_SE).handleId_);
	sndM.AdjustVolume("JustDodge", JUST_DODGE_SE_VOL);

}

void Game::InitEffect(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	EffectManager& efcM = EffectManager::GetInstance();

	//剣
	efcM.Add("Sword", rsM.Load(ResourceManager::SRC::SWORD_EFC).handleId_);
	//ダメージ
	efcM.Add("Damage", rsM.Load(ResourceManager::SRC::DAMAGE_EFC).handleId_);
	//攻撃チャージ
	efcM.Add("Charge", rsM.Load(ResourceManager::SRC::CHARGE_ATK_EFC).handleId_);
}

void Game::InitShader(void)
{
	//ブラー(回避用)
	//PS
	dodgeMaterial_ = std::make_unique<PixelMaterial>("JustDodgePS.cso", DODGE_NUM_BUFF_PS);
	//拡散光
	dodgeMaterial_->AddConstBuf({ 1.0f,1.0f,1.0f,1.0f });
	//時間
	dodgeMaterial_->AddConstBuf({ 0.0f,0.0f ,0.0f,0.0f });
	//画面X・Y・強さ・半径
	dodgeMaterial_->AddConstBuf({ Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y ,DODGE_EFFECT_RATE,DODGE_EFFECT_RADIUS });

	dodgeMaterial_->SetTextureBuf(11, ResourceManager::GetInstance().Load(ResourceManager::SRC::FOCUS_IMG).handleId_);

	dodgeRender_ = std::make_unique<PixelRenderer>(*dodgeMaterial_);

	dodgeRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// ポストエフェクト用スクリーン
	dodgeScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void Game::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();
	InputManager& inpM = InputManager::GetInstance();

#pragma region シーン遷移(ルール)
	//プレイヤーが死んでいたら
	if (!player_->IsAlive()) {
		//BGM念のため両方停止
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(std::make_shared<GameOver>());
	}
#pragma endregion

	//更新
	//演出内容があるならば
	if (direction_ != nullptr) {
		bool isCutSceneEnd = false;
		isCutSceneEnd = direction_->Update();	//演出更新

		//演出終了時処理
		if (isCutSceneEnd) {
			direction_->Release();
			direction_ = nullptr;	//解放
		}
	}
	else {
		GameUpdate();	//通常ゲーム更新
	}
}

void Game::GameUpdate(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();
	Camera& camera = scM.GetCamera();
	
	//敵がいなくなったら
	if (enemy_->GetEnemys().size() <= 0) {
		sndM.Stop(nowBgmStr_);
		sndM.Stop(switchBgmStr_);
		//シーン遷移
		scM.ChangeScene(std::make_shared<GameClear>());
	}

	//ポーズシーン遷移
	if (inpM.IsTrigerrDown("pause")) {
		//シーン追加(一つ次へ)
		scM.PushScene(std::make_shared<PauseScene>());
	}

#pragma region 基礎アプデ
	//プレイヤー
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

	//ステージ
	stage_->Update();

	CollisionManager::GetInstance().UpdateColliders();

#pragma endregion

#pragma region BGM
	//敵の状態(戦闘・それ以外)のトリガ
	if (enemy_->IsBattleStateChanged()) {
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
		sndM.AdjustVolume(nowBgmStr_, (BGM_VOL_MAX - nextBgmVol_));	//現在のBGMは音量を下げる

		//もしボリュームが最大値以上なら
		if (nextBgmVol_ >= BGM_VOL_MAX) {
			//音量を最大値に
			nextBgmVol_ = BGM_VOL_MAX;	
			//終了処理
			FinishSwitchBgm();
		}
	}
#pragma endregion

#pragma region カメラ
	//カメラの設定
	if (!player_->IsUseAbility()) {
		camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象の更新
	}
	else {
		VECTOR abilityFollow = player_->GetPos();
		abilityFollow.y += CAMERA_FOLLOW_DIFF_Y_ABILITY;
		camera.SetFollow(abilityFollow, player_->GetQua());		//追従対象の更新
	}
	
#pragma endregion
}

void Game::Draw(void)
{
	stage_->Draw();
	enemy_->Draw();
	player_->Draw();

	//メニューボタンの表示
	UIManager2d::GetInstance().Draw(MENU_BTN);

	//スロー時(回避成功時)
	if (isSlowEffect_) {
		//描画
		DrawDodgeEffect();	//回避用のポストエフェクト
	}

	//演出がある場合
	if (direction_ != nullptr) {
		direction_->Draw();
	}
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
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.Stop("NomalBgm");	//今まで流していたものを停止
	sndM.Stop("BattleBgm");	//今まで流していたものを停止
	CollisionManager::GetInstance().DeleteAllCollider();
}

void Game::Reset(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, BGM_VOL);	//前シーンに戻るのでBGMの音量を復活
	//とりあえずメニューからの復帰時は追従に
	//メニュー開く直前に変える可能性大
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void Game::StartBossFaze(void)
{
	SoundManager& sndM = SoundManager::GetInstance();

	sndM.Stop("NomalBgm");	//今まで流していたものを停止
	sndM.Stop("BattleBgm");	//今まで流していたものを停止

	//演出初期設定
	PlayCutScene(CUT_SCENE_TYPE::APPEAR_BOSS);
}

void Game::PlayCutScene(const CUT_SCENE_TYPE& _type)
{
	switch (_type) {
	case CUT_SCENE_TYPE::APPEAR_BOSS:
		direction_ = std::make_unique<AppearBoss>(*this, *player_, *enemy_);
		break;

	case CUT_SCENE_TYPE::DEATH_PLAYER:
		direction_ = std::make_unique<DeathPlayer>();
		break;

	case CUT_SCENE_TYPE::DEATH_BOSS:
		direction_ = std::make_unique<DeathBoss>(*enemy_);
		break;
	}

	direction_->Init();
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
	//今まで流していたものを停止
	sndM.Stop(nowBgmStr_);	
	//現在のBGM名と切り替え後のBGM名の切り換え
	auto ret = nowBgmStr_;
	nowBgmStr_ = switchBgmStr_;
	switchBgmStr_ = ret;
	//初期化
	nextBgmVol_ = 0;
}


void Game::StartSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	//スロー演出準備
	slowCnt_ = 0;
	isSlowEffect_ = true;
	//更新スピードを50％に設定
	scM.SetUpdateSpeedRate(SLOW_SPEED_PERCENT);
	//敵もそれに対応
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent());
}

void Game::EndSlow(void)
{
	auto& scM = SceneManager::GetInstance();
	isSlowEffect_ = false;
	//更新処理を100％にもどす
	scM.SetUpdateSpeedRate(NOMAL_SPEED_PERCENT);
	enemy_->SetAnimSpeedRate(scM.GetUpdateSpeedRatePercent());
}

void Game::StartBgm(std::string _bgmName)
{
	SoundManager::GetInstance().Play(_bgmName);
	nowBgmStr_ = _bgmName;
	switchBgm_ = false;
}
