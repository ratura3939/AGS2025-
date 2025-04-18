#include "EnemyBase.h"

void EnemyBase::SetPram(void)
{
	pos_ = { 0.0f,0.0f,1000.0f };
}

void EnemyBase::Update(void)
{
}

void EnemyBase::Draw(void)
{
	//•`‰æˆ—
	DrawSphere3D(pos_, 30, 10, 0xffff00, 0xffff00, false);
}
