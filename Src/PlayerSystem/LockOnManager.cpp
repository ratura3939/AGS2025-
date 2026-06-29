#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/GameSystem/EnemyManager.h"
#include"../Manager/GameSystem/PlayerManager.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Scene/Main/Game.h"
#include "LockOnManager.h"

LockOnManager::LockOnManager(Game& _gameScene, PlayerManager& _player, EnemyManager& _enemyInfo) :
	scene_(_gameScene)
	,master_(_player)
	,targets_(_enemyInfo)
	,nearTargetNum_(-1)
	,preNearTargetNum_(-1)
	,isLockOn_(false)
{
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
	master_.RedyLockOn();						//ロックオン開始を通知
	targets_.LokedOn(nearTargetNum_);			//ターゲットにロックオンを通知
	camera.ChangeMode(Camera::MODE::LOCKON);	//カメラをロックオン状態に
	SoundManager::GetInstance().Play("RockOn");	//SEの再生

	isLockOn_ = true;	//状態を保存
}

void LockOnManager::LockOff(void)
{
	//無駄な処理をしないため
	if (!isLockOn_)return;

	Camera& camera = SceneManager::GetInstance().GetCamera();
	master_.RedyLockOff();		//ロックオン解除の通知
	targets_.NoTargetEnemy();	//ターゲットに解除を通知
	camera.ChangeMode(Camera::MODE::FOLLOW);	//追従に戻す
	//ゾーンを続かせないために
	scene_.EndSlow();
	//対象をキャンセルしたとみなし初期化する
	nearTargetNum_ = -1;

	isLockOn_ = false;	//状態の保存
}

const int LockOnManager::DecideRockEnemy(void)
{
	return targets_.GetNearEnemyNum(master_.GetPos());
}
