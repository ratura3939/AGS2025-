#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Utility/Utility.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& _playerPos):pPos_(_playerPos)
{
	color_ = 0xffffff;
	update_ = &EnemyBase::UpdateNomal;
}

EnemyBase::~EnemyBase(void)
{
}

const bool EnemyBase::Init(void)
{

	SetPram();
	modelId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_MDL).handleId_;
	if (modelId_ == -1) {
		return false;
	}
	scl_ = { CHARA_SCALE,CHARA_SCALE ,CHARA_SCALE };

	pos_ = { 0.0f,0.0f,1000.0f };
	rot_ = { 0.0f,0.0f,-1.0f };
	quaRotLocal_ = Quaternion::Euler(0.0f, Utility::Deg2RadF(INIT_MODEL_ROT), 0.0f);

	UpdateRotQuat();
	return true;
}

void EnemyBase::SetColor(int _color)
{
	color_ = _color;
}

void EnemyBase::SetPos(VECTOR _pos)
{
	pos_ = _pos;
}

void EnemyBase::DrawDebug(void)
{
	VECTOR forwardDif = { 0,0,100 };

	VECTOR fowardPos = VAdd(pos_, characterRotY_.PosAxis(forwardDif));
	DrawSphere3D(fowardPos, 8, 8, 0xff5500, 0xff0000, false);
}

void EnemyBase::Update(void)
{
	(this->*update_)(pPos_);
	//共通更新
	Rotation();
	UpdateRotQuat();
}

void EnemyBase::SetPram(void)
{
	//各敵たち
	//後々Jsonやったら楽になるかも？
}

void EnemyBase::UpdateNomal(const VECTOR& _pPos)
{

}

void EnemyBase::UpdateSearch(const VECTOR& _pPos)
{
}

void EnemyBase::UpdateBattle(const VECTOR& _pPos)
{
}

