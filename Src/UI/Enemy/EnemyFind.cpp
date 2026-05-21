#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "EnemyFind.h"

EnemyFind::EnemyFind(VECTOR& _followPos, EnemyBase::ENEMY_STATE& _state):UIBase(_followPos),eState_(_state)
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
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	suspectStr_ = _master + "Suspect";
	findStr_ = _master + "Find";

	//「？」のUI登録
	uiM.Add(suspectStr_, rsM.Load(ResourceManager::SRC::SUSPECT_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::ZOOM_IN, UI_DIMENSION::DIMENSION_3);	//追加(拡大演出)
	uiM.SetUIInfo(suspectStr_, drawFollowPos_);		//基礎情報の設定
	uiM.SetUIDirectionParam(suspectStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM, SUSPECT_EXT_ACC, SUSPECT_EXT_MAX, SUSPECT_EXT_MIN);	//拡大演出のパラメータ設定

	//「！」のUI登録
	uiM.Add(findStr_, rsM.Load(ResourceManager::SRC::FIND_IMG).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UI_DIMENSION::DIMENSION_3);	//追加
	uiM.SetUIInfo(findStr_, drawFollowPos_, FIND_UI_DRAW_SIZE_MAX);	//基礎情報の設定

	return true;
}

bool EnemyFind::Update(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	std::string useStr = "";	//更新に使用する文字列

	if (eState_==EnemyBase::ENEMY_STATE::SEARCH) {
		//見つけていないとき
		//「？」を設定
		useStr = suspectStr_;
	}
	if(eState_ == EnemyBase::ENEMY_STATE::BATTLE&& findUICnt_<= FIND_UI_DRAW_TIME) {
		//見つけているとき
		//「！」を設定
		useStr = findStr_;
		findUICnt_++;	//カウンター更新
	}

	//更新処理
	if (useStr != "") {
		uiM.SetPos(useStr, drawFollowPos_);
		uiM.Update(useStr);
	}
	
	return true;
}

void EnemyFind::Draw(void)
{

	UIManager2d& uiM = UIManager2d::GetInstance();

	if (eState_ == EnemyBase::ENEMY_STATE::SEARCH) {
		//見つけていないとき
		uiM.Draw(suspectStr_);	//「？」描画
	}
	if (eState_ == EnemyBase::ENEMY_STATE::BATTLE && findUICnt_ <= FIND_UI_DRAW_TIME) {
		//見つけているとき
		uiM.Draw(findStr_);		//「！」描画
	}
}

void EnemyFind::Reset(void)
{
	findUICnt_ = 0.0f;
	UIManager2d::GetInstance().ResetUpdate(suspectStr_, UIManager2d::UI_DIRECTION_GROUP::ZOOM);
}
