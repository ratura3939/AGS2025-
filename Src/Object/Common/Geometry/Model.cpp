#include<DxLib.h>
#include"Sphere.h"
#include "Model.h"

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId) 
    : Geometry(_pos, _rot)
	, modelId_(_modelId)
{
}

Model::~Model(void)
{
}

const bool Model::IsHit(Geometry& _geo)
{
    return _geo.IsHit(*this);
}

const bool Model::IsHit(Sphere& _sphere)
{
    auto col = MV1CollCheck_Sphere(modelId_, -1, _sphere.GetPos(), _sphere.GetRadius());
    return col.HitNum >= 1;
}

const bool Model::IsHit(Model& _model)
{
    //èoóàÇ»Ç¢
    return false;
}

void Model::DebugDraw(void)
{
}
