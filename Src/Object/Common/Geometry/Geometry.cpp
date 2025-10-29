#include "Geometry.h"

Geometry::~Geometry(void)
{
}

Geometry::Geometry(const VECTOR& _pos, const Quaternion& _rot)
	: colPos_(_pos)
	, colRot_(_rot)
{
}
