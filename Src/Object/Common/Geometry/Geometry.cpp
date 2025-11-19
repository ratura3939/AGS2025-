#include"../../../Utility/Utility.h"
#include "Geometry.h"

Geometry::~Geometry(void)
{
}

void Geometry::RedyCollision(void)
{
}

Geometry::Geometry(const VECTOR& _pos, const Quaternion& _rot)
	: colPos_(_pos)
	, colRot_(_rot)
	, hitNormal_(Utility::VECTOR_ZERO)
	, hitPoint_(Utility::VECTOR_ZERO)
{
}
