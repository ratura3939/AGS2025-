#include"../../../Utility/Utility.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include "Model.h"
#include "Line.h"

Line::Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _startPos, const VECTOR& _endPos)
    : Geometry(_pos, _rot)
    , startPos_(_startPos)
    , endPos_(_endPos)
	, length_(Utility::MagnitudeF(VSub(_endPos, _startPos)))
{
}

Line::~Line(void)
{
}

const bool Line::IsHit(Geometry& _geo)
{
    return _geo.IsHit(*this);
}

const bool Line::IsHit(Line& _line)
{
    //Œã‰ñ‚µ
    return false;
}

const bool Line::IsHit(Sphere& _sphere)
{
    return _sphere.IsHit(*this);
}

const bool Line::IsHit(Capsule& _capsule)
{
    return _capsule.IsHit(*this);
}

const bool Line::IsHit(Cube& _cube)
{
    return _cube.IsHit(*this);
}

const bool Line::IsHit(Model& _model)
{
    return _model.IsHit(*this);
}

void Line::DebugDraw(void)
{
}
