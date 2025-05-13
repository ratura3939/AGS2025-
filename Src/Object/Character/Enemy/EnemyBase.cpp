#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Utility/Utility.h"
#include "EnemyBase.h"

const bool EnemyBase::Init(void)
{

	SetPram();
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_MDL).handleId_;
	if (modelId_ == -1)return false;
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };

	pos_ = { 0.0f,0.0f,1000.0f };
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);
	
	color_ = 0xffffff;
	Init3DPram();
}

void EnemyBase::SetColor(int _color)
{
	color_ = _color;
}

void EnemyBase::SetPos(VECTOR _pos)
{
	pos_ = _pos;
}

void EnemyBase::Update(void)
{
}

void EnemyBase::Draw(void)
{
	//ï`âÊèàóù
	DrawSphere3D(pos_, 30, 10, color_, color_, false);
}

void EnemyBase::SetPram(void)
{
}
