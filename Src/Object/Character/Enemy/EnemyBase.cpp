#include "EnemyBase.h"

void EnemyBase::SetPram(void)
{
	pos_ = { 0.0f,0.0f,1000.0f };
	color_ = 0xffffff;
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
