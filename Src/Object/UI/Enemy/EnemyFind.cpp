#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "EnemyFind.h"

EnemyFind::EnemyFind(void)
{
	suspectEx_ = 0.0f;
	findUICnt_ = 0.0f;
}

EnemyFind::~EnemyFind(void)
{
}

bool EnemyFind::Init(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();

	suspectImg_ = rsM.Load(ResourceManager::SRC::SUSPECT_IMG).handleId_;
	findImg_ = rsM.Load(ResourceManager::SRC::FIND_IMG).handleId_;
	return true;
}

bool EnemyFind::Update(void)
{
	//視界内
	suspectEx_ += SUSPECT_EXT_ACC;

	//視界外
	suspectEx_ -= SUSPECT_EXT_ACC;
	if (suspectEx_ <= 0.0f) {
		suspectEx_ = 0.0f;
	}

	findUICnt_ += SceneManager::GetInstance().GetUpdateSpeedRate_();

	return true;
}

void EnemyFind::Draw(void)
{
	//if (state_ == ENEMY_STATE::SEARCH) {
	//	//「?」マークの描画
	//	DrawBillboard3D(drawPos_, 0.5f, 0.5f, suspectEx_, 0.0f, suspectImg_, true);
	//}
	//if (state_ == ENEMY_STATE::BATTLE && findUICnt_ <= FIND_UI_DRAW_TIME) {
	//	//「!」マークの描画
	//	DrawBillboard3D(drawPos_, 0.5f, 0.5f, FIND_UI_DRAW_SIZE, 0.0f, findImg_, true);
	//}
}

void EnemyFind::Reset(void)
{
	suspectEx_ = 0.0f;
	findUICnt_ = 0.0f;
}
