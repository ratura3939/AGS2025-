#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyTargetting.h"


EnemyTargetting::EnemyTargetting(VECTOR& _followPos):
	UIBase(_followPos)
	,isLocked_(false)
{
}

EnemyTargetting::~EnemyTargetting(void)
{
}

bool EnemyTargetting::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	noticeStr_ = _master + "LockNotice";
	lockStr_ = _master + "Locked";

	//ロックオン可能通知UI
	uiM.Add(noticeStr_, rsM.Load(ResourceManager::SRC::ANNOUNCE_LOCKON_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::FLASHING, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(noticeStr_, drawFollowPos_, TARGET_UI_DRAW_SIZE);
	uiM.SetUIDirectionParam(noticeStr_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, NOTICE_ALPHA_ACC, NOTICE_ALPHA_MAX, NOTICE_ALPHA_MIN);

	//ロックオンUI
	uiM.Add(lockStr_, rsM.Load(ResourceManager::SRC::LOCKON_IMG ).handleId_, UIManager2d::UI_DIRECTION_2D::UP_DOWN, UI_DIMENSION::DIMENSION_3);
	uiM.SetUIInfo(lockStr_, drawFollowPos_, TARGET_UI_DRAW_SIZE);
	uiM.SetUIDirectionParam(lockStr_, UIManager2d::UI_DIRECTION_GROUP::MOVE, LOCK_MOVE_SPEED, LOCK_MOVE_MAX, LOCK_MOVE_MIN);

	return true;
}

bool EnemyTargetting::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	std::string useStr = noticeStr_;	//使用するUIの登録名

	//ロックオンされていたら
	if (isLocked_) {
		useStr = lockStr_;	//ロックオンに切り替え
	}

	//更新処理
	uiM.SetPos(useStr, drawFollowPos_);	//位置設定
	uiM.Update(useStr);	//更新

	return true;
}

void EnemyTargetting::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	std::string useStr = noticeStr_;	//使用するUIの登録名

	//ロックオンされていたら
	if (isLocked_) {
		useStr = lockStr_;	//ロックオンに切り替え
	}

	uiM.Draw(useStr);	//描画
}

void EnemyTargetting::Reset(void)
{
}
