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

	constexpr float SCALE_SKEL = 0.2f;
	constexpr float SCALE_PALTE_NUM = 0.4f;
	constexpr float POS_AJUST = 100.0f;

	plateStr_ = _master + "CounterPlate";
	numberStr_ = _master + "UseNumber";
	SkeltonConterStr_ = _master + "SkeltonIcon";

	//プレート
	uiM.Add(plateStr_, rsM.Load(ResourceManager::SRC::PLATE_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(plateStr_, drawFollowPos_, SCALE_PALTE_NUM);
	//数字
	auto numPos = drawFollowPos_;
	numPos.x += POS_AJUST;
	uiM.Add(numberStr_, -1, UIManager2d::UI_DIRECTION_2D::UP_DOWN, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(numberStr_, numPos, SCALE_PALTE_NUM);
	uiM.SetUIDirectionParam(numberStr_, UIManager2d::UI_DIRECTION_GROUP::MOVE, 2.0f, 20.0f, -20.0f);			//詳細設定

	//アイコン×
	auto iconPos = drawFollowPos_;
	iconPos.x -= POS_AJUST/2;
	uiM.Add(SkeltonConterStr_, rsM.Load(ResourceManager::SRC::SKELETON_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UI_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(SkeltonConterStr_, iconPos, SCALE_SKEL);

	golemIcon_ = rsM.Load(ResourceManager::SRC::GOLEM_IMG).handleId_;

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
	uiM.Draw(plateStr_);
	uiM.Draw(SkeltonConterStr_);
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

void EnemyCount::SetIconImg(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetImage(SkeltonConterStr_, golemIcon_);
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
	uiM.Update(numberStr_);
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
