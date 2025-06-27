#include"../../../Manager/Generic/ResourceManager.h"
#include "EnemyTargetting.h"

EnemyTargetting::EnemyTargetting(void)
{
	isLocked_ = false;
	isLockTarget_ = false;
}

EnemyTargetting::~EnemyTargetting(void)
{
}

bool EnemyTargetting::Init(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	lockNoticeImg_ = rsM.Load(ResourceManager::SRC::ANNOUNCE_LOCKON_IMG).handleId_;
	lockOnImg_ = rsM.Load(ResourceManager::SRC::LOCKON_IMG).handleId_;
	return true;
}

bool EnemyTargetting::Update(void)
{
	return true;
}

void EnemyTargetting::Draw(void)
{
	int rockUIImg = lockNoticeImg_;
	//ƒƒbƒNƒIƒ“‚³‚ê‚Ä‚¢‚½‚ç
	if (isLocked_) {
		//UI‰æ‘œ‚Ì·‚µ‘Ö‚¦
		rockUIImg = lockOnImg_;
	}
	DrawBillboard3D(drawPos_, 0.5f, 0.5f, LOCK_UI_DRAW_SIZE, 0.0f, rockUIImg, true);
}

void EnemyTargetting::Reset(void)
{
}
