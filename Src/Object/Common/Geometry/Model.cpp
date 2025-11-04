#include<DxLib.h>
#include"Sphere.h"
#include"Capsule.h"
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
    hitSphereInfo_ = MV1CollCheck_Sphere(modelId_, -1, _sphere.GetPos(), _sphere.GetRadius());
    bool isHit = hitSphereInfo_.HitNum >= 1;

    return isHit;
}

const bool Model::IsHit(Capsule& _capsule)
{
    hitCapsuleInfo_ = MV1CollCheck_Capsule(modelId_, -1, _capsule.GetPosTop(), _capsule.GetPosBottom(), _capsule.GetRadius());
    bool isHit = hitCapsuleInfo_.HitNum >= 1;

    return isHit;
}

const bool Model::IsHit(Model& _model)
{
    //o—ˆ‚È‚¢
    return false;
}

void Model::DebugDraw(void)
{
}
