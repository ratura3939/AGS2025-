#include"../../../Application.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/EnemyManager.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Renderer/PixelMaterial.h"
#include"../../../Renderer/PixelRenderer.h"
#include "DeathBoss.h"

namespace {
	const int DIRECTION_TIME = 240;
	const int BOSS_IDX = 0;		//ボスの配列番号(ボス単体のため必ず0)
	const VECTOR CAMERA_RELATIVE = { 0.0f,400.0f,600.0f };
	const VECTOR CAMERA_FOCUS_RELATIVE = { 0.0f,300.0f,0.0f };

	const int FLASH_BUFF_NUM_PS = 2;				//バッファ総数
	const FLOAT4 WHITE = { 0.6f,0.6f,0.6f,1.0f };	//白
	const int FLASH_POWER_BUFF_NUM = 1;				//フラッシュの強さを伝えるバッファ番号
	const int FLASH_NUM = 6;		//フラッシュ回数
	const float FLASH_MAX_RATE = 1.0f;	//フラッシュ最高値
}

DeathBoss::DeathBoss(EnemyManager& _enemy):
	enemy_(_enemy)
	,directionCounter_(0)
	,flashPower_(0.0f)
{
}

DeathBoss::~DeathBoss(void)
{
}

void DeathBoss::DoInit(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();
	const VECTOR& bossPos = enemy_.GetPos(BOSS_IDX);
	const Quaternion& bossQua = enemy_.GetQua(BOSS_IDX);

	camera.SetFollow(bossPos, bossQua);
	camera.SetFocusPos(VAdd(bossPos, CAMERA_FOCUS_RELATIVE));
	camera.SetPos(VAdd(bossPos, bossQua.PosAxis(CAMERA_RELATIVE)));

	camera.ChangeMode(Camera::MODE::FIXED_POINT);

	//走査線
	//PS
	flashMaterial_ = std::make_unique<PixelMaterial>("Flash.cso", FLASH_BUFF_NUM_PS);
	//拡散光
	flashMaterial_->AddConstBuf(WHITE);
	//時間
	flashMaterial_->AddConstBuf({ flashPower_,0.0f,0.0f,0.0f });

	flashRender_ = std::make_unique<PixelRenderer>(*flashMaterial_);
	flashRender_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });
	// ポストエフェクト用スクリーン
	flashScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	SoundManager& sndM = SoundManager::GetInstance();
	sndM.Add(SoundManager::TYPE::SE, "Flash", ResourceManager::GetInstance().Load(ResourceManager::SRC::BOSS_DEATH_SE).handleId_);
}

bool DeathBoss::DoUpdate(void)
{
	directionCounter_++;
	if (directionCounter_ > DIRECTION_TIME) {
		return true;
	}

	//ポストエフェクト更新
	int flashInterval = DIRECTION_TIME / FLASH_NUM;
	int flashRateForInterval = (directionCounter_ % flashInterval);
	flashPower_ = static_cast<float>(flashRateForInterval) / static_cast<float>(flashInterval);
	flashPower_ = sin(flashPower_ * DX_PI_F);

	if (flashPower_ == FLASH_MAX_RATE) {
		SoundManager::GetInstance().Play("Flash");
	}

	flashMaterial_->SetConstBuf(FLASH_POWER_BUFF_NUM, { flashPower_,0.0f,0.0f,0.0f});	//フラッシュ強さ設定

	//アニメーションのみ更新
	enemy_.UpdateAnim();

	return false;
}

void DeathBoss::DoDraw(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	SetDrawScreen(flashScreen_);

	DrawGraph(0, 0, mainScreen, false);
	flashRender_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, flashScreen_, false);
}

void DeathBoss::DoRelease(void)
{
}

void DeathBoss::FinishDirection(void)
{
	enemy_.DeleteBoss();	//ボスの削除
	SoundManager::GetInstance().Stop("Flash");
}
