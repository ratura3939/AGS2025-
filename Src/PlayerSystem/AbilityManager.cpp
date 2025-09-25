#include<DxLib.h>
#include"../Object/Stage/StageManager.h"
#include "AbilityManager.h"

//ローカル定数
namespace {
	const VECTOR NONE_COLOR = { 0.0f,0.0f,0.0f };
	const VECTOR MAGNET_COLOR = { 1.0f,0.0f,0.0f };
	const VECTOR LOCK_TIME_COLOR = { 1.0f,1.0f,0.0f };
}

AbilityManager::AbilityManager(StageManager& _stage):stage_(_stage)
{
	useAbility_ = ABILITY_TYPE::TIME_LOCK;
	isUsingAbility_ = false;
}

AbilityManager::~AbilityManager(void)
{
}

void AbilityManager::Update(void)
{

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

void AbilityManager::ChangeAbility(void)
{
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
