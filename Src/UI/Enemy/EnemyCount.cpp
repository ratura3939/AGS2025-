#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyCount.h"

EnemyCount::EnemyCount(VECTOR& _followPos):UIBase(_followPos)
{
}

EnemyCount::~EnemyCount(void)
{
}

bool EnemyCount::Init(const std::string& _master)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	plateStr_ = _master + "CounterPlate";
	numberStr_ = "UseNumber";

	uiM.Add(plateStr_, rsM.Load(ResourceManager::SRC::PLATE_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(plateStr_, drawFollowPos_);

	uiM.Add(numberStr_, -1, UIManager2d::UI_DIRECTION_2D::NOMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(numberStr_, drawFollowPos_);

	update_ = &EnemyCount::NomalUpdate;
	return true;
}

bool EnemyCount::Update(void)
{
	(this->*update_)();
	return false;
}

void EnemyCount::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.Draw(numberStr_);
}

void EnemyCount::Reset(void)
{
}

void EnemyCount::SetNumImg(int _img)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetImage(numberStr_, _img);
}

void EnemyCount::Action(void)
{
	if (state_ == COUNTER_STATE::NOAML) {
		StartAction();
		update_ = &EnemyCount::ActionUpdate;
	}
}

void EnemyCount::StartAction(void)
{
	//アクション状態に
	state_ = COUNTER_STATE::ACTION;
	//もしかしたら移動はさせないかも
	//プレートを中心に移動させる手順を開始

	//目標地点は画面中央


}

void EnemyCount::NomalUpdate(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//表示項目を常にぴょこぴょこさせる
	
}

void EnemyCount::ActionUpdate(void)
{
	(this->*actionUpdate_)();
}

void EnemyCount::UpdateMove(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//目標地点に行くまで徐々にスケールを変更させる

	//目標位置についたら演出用の更新に
	actionUpdate_ = &EnemyCount::UpdateDirection;
}

void EnemyCount::UpdateDirection(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//演出が終了したら次の状態に(移動つけるならMOVEにしないならNOMAL状態に戻す)
	actionUpdate_ = &EnemyCount::UpdateMove;
}
