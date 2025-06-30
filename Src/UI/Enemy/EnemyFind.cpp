#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyFind.h"

EnemyFind::EnemyFind(void)
{
	findUICnt_ = 0.0f;
}

EnemyFind::~EnemyFind(void)
{
}

bool EnemyFind::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();

	suspectStr_ = _master + "Suspect";
	findStr_ = _master + "Find";

	uiM.Add(suspectStr_, rsM.Load(ResourceManager::SRC::SUSPECT_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::ZOOM_IN);
	uiM.SetUIInfo(suspectStr_, drawPos_);
	uiM.SetUIDirectionPram(suspectStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, SUSPECT_EXT_ACC, SUSPECT_EXT_MAX, 1.0f);

	uiM.Add(findStr_, rsM.Load(ResourceManager::SRC::FIND_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL);
	uiM.SetUIInfo(findStr_, drawPos_);

	return true;
}

bool EnemyFind::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.Update(suspectStr_);

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

	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.Draw(suspectStr_);
	uiM.Draw(findStr_);
}

void EnemyFind::Reset(void)
{
	findUICnt_ = 0.0f;
}
