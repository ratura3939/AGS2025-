#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/GameSystem/PlayerManager.h"
#include"../Scene/Main/Game.h"
#include "LockOnManager.h"

LockOnManager::LockOnManager(Game& _gameScene, PlayerManager& _player, EnemyManager& _enemyInfo) :scene_(_gameScene), master_(_player), targets_(_enemyInfo)
{
	nearTargetNum_ = -1;
	preNearTargetNum_ = -1;
	isLockOn_ = false;
}

LockOnManager::~LockOnManager(void)
{
}

void LockOnManager::Update(void)
{
	//対象者の選択
	preNearTargetNum_ = nearTargetNum_;	//保存
	nearTargetNum_ = DecideRockEnemy();	//検索

	//非ロックオン時
	if (!isLockOn_) {
		//ロックオン対象が変わった時
		if (preNearTargetNum_ != nearTargetNum_) {
			//対象の更新
			targets_.SetTargetEnemy(nearTargetNum_);
		}
		//対象となる敵がいないとき
		if (nearTargetNum_ < 0) {
			targets_.NoTargetEnemy();
		}
	}
	else {
		//ロックオン時対象となる敵がいなかったら
		if (nearTargetNum_ < 0) {
			LockOff();
			return;
		}
		Camera& camera = SceneManager::GetInstance().GetCamera();
		camera.SetLockPos(targets_.GetPos(nearTargetNum_));	//ロックオン対象の設定
	}

	//カメラの注視点設定
	SetTaergetPos4Camera();
}

const bool LockOnManager::CanLockOn(void)
{
	return targets_.GetNearEnemyNum(master_.GetPos()) >= 0;
}

void LockOnManager::LockOn(void)
{
	//無駄な処理をしないため
	if (isLockOn_)return;
	if (nearTargetNum_ < 0)return;

	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.SetLockPos(targets_.GetPos(nearTargetNum_));	//ロックオン対象の設定
	master_.RedyLockOn();
	targets_.LokedOn(nearTargetNum_);
	camera.ChangeMode(Camera::MODE::LOCKON);

	isLockOn_ = true;
}

void LockOnManager::LockOff(void)
{
	//無駄な処理をしないため
	if (!isLockOn_)return;

	Camera& camera = SceneManager::GetInstance().GetCamera();
	master_.RedyLockOff();
	targets_.NoTargetEnemy();
	camera.ChangeMode(Camera::MODE::FOLLOW);
	//ゾーンを続かせないために
	scene_.ChangeActionDirec(Game::ACTION_DIRECTION::NOMAL);
	scene_.EndSlow();
	//対象をキャンセルしたとみなし初期化する
	nearTargetNum_ = -1;

	isLockOn_ = false;
}

void LockOnManager::SetTaergetPos4Camera(void)
{
	Camera& camera = SceneManager::GetInstance().GetCamera();

	////ロックオンをしていたら
	//if (isLockOn_) {
	//	//ロックオン対象に敵を設定
	//	camera.SetFocusPos(master_.GetFocusPoint());//注視点の更新
	//	camera.SetFocusPos(master_.GetPos());//注視点の更新
	//}
	//else {
	//	//追従対象にプレイヤーを設定
	//	camera.SetRockPos(targets_.GetPos(nearTargetNum_));	//ロックオン対象の設定
	//}
}

const int LockOnManager::DecideRockEnemy(void)
{
	return targets_.GetNearEnemyNum(master_.GetPos());
}
