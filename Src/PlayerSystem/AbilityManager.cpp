#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Decoration/UIManager2d.h"
#include"../Object/Stage/StageManager.h"
#include "AbilityManager.h"

const std::string AbilityManager::UI_ABILITY_MGNET = "MagnetIcon";
const std::string AbilityManager::UI_ABILITY_LOCK_TIME = "LockTimeIcon";
const VECTOR AbilityManager::ABILITY_ICON_POS= { 170.0f,230.0f,0.0f };

//ローカル定数
namespace {
	const VECTOR NONE_COLOR = { 0.0f,0.0f,0.0f };
	const VECTOR MAGNET_COLOR = { 1.0f,0.0f,0.0f };
	const VECTOR LOCK_TIME_COLOR = { 1.0f,1.0f,0.0f };
}

AbilityManager::AbilityManager(StageManager& _stage):stage_(_stage)
{
	useAbility_ = ABILITY_TYPE::LOCK_TIME;
	isUsingAbility_ = false;

	auto& resM = ResourceManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	//マグネットアイコン
	uiM.Add(UI_ABILITY_MGNET, resM.Load(ResourceManager::SRC::ABILITY_MAGNET_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_MGNET, ABILITY_ICON_POS, UI_EX);

	//タイムロックアイコン
	uiM.Add(UI_ABILITY_LOCK_TIME, resM.Load(ResourceManager::SRC::ABILITY_LOCK_TIME_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_LOCK_TIME, ABILITY_ICON_POS, UI_EX);

	iconNames_[static_cast<int>(ABILITY_TYPE::MAGNET)] = UI_ABILITY_MGNET;
	iconNames_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = UI_ABILITY_LOCK_TIME;
}

AbilityManager::~AbilityManager(void)
{
}

void AbilityManager::Update(void)
{

}

void AbilityManager::Draw(void)
{
	if (useAbility_ != ABILITY_TYPE::NONE && useAbility_ != ABILITY_TYPE::MAX) {
		UIManager2d::GetInstance().Draw(iconNames_[static_cast<int>(useAbility_)]);
	}
}

void AbilityManager::RedyAbility(void)
{
	//使用中に
	isUsingAbility_ = true;

	//色の設定
	VECTOR col = GetAbilityColor(useAbility_);
	stage_.SetAbility({ col.x,col.y,col.z ,1.0f });
}

void AbilityManager::UseAbility(void)
{
}

void AbilityManager::EndUsingAbility(void)
{
	//使用終了
	isUsingAbility_ = false;

	//付与色をなくす
	VECTOR col = NONE_COLOR;
	stage_.SetAbility({ col.x,col.y,col.z ,1.0f });
}

void AbilityManager::ChangeAbility(const ABILITY_TYPE _type)
{
	useAbility_ = _type;
}

VECTOR AbilityManager::GetAbilityColor(const ABILITY_TYPE _type)
{
	VECTOR ret;

	if (_type == ABILITY_TYPE::MAGNET) {
		ret = MAGNET_COLOR;
	}
	else {
		ret = LOCK_TIME_COLOR;
	}

	return ret;
}
