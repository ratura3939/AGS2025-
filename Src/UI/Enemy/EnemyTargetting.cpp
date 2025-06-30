#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyTargetting.h"

EnemyTargetting::EnemyTargetting(void)
{
	isLocked_ = false;
	isLockTarget_ = false;
}

EnemyTargetting::~EnemyTargetting(void)
{
}

bool EnemyTargetting::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();

	noticeStr_ = _master + "LockNotice";
	lockStr_ = _master + "Locked";
	uiM.Add(noticeStr_, rsM.Load(ResourceManager::SRC::ANNOUNCE_LOCKON_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::FLASHING);
	uiM.SetUIInfo(noticeStr_, drawPos_);
	uiM.SetUIDirectionPram(noticeStr_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, 1.0f, 255.0f, 1.0f);

	uiM.Add(lockStr_, rsM.Load(ResourceManager::SRC::LOCKON_IMG ).handleId_, UIManager2d::UI_DIRECTION_2D::UP_DOWN);
	uiM.SetUIInfo(lockStr_, drawPos_);
	uiM.SetUIDirectionPram(lockStr_, UIManager2d::UI_DIRECTION_GROUP::GRADUALLY, 1.0f, 255.0f, 1.0f);

	return true;
}

bool EnemyTargetting::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//ロックオンされていたら
	if (isLocked_) {
		uiM.Update(lockStr_);
	}
	else {
		uiM.Update(noticeStr_);
	}
	return true;
}

void EnemyTargetting::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//ロックオンされていたら
	if (isLocked_) {
		uiM.Draw(lockStr_);
	}
	else {
		uiM.Draw(noticeStr_);
	}
}

void EnemyTargetting::Reset(void)
{
}
