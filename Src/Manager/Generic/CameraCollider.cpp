#include"Camera.h"
#include"../../Object/Common/Collider.h"
#include"../../Object/Common/Geometry/Sphere.h"
#include "CameraCollider.h"

namespace {
	const float SPHERE_RADIUS = 15.0f;
}

CameraCollider::CameraCollider(Camera& _camera)
	: camera_(_camera)
{
}

CameraCollider::~CameraCollider(void)
{
}

void CameraCollider::Draw(void)
{
	//•`‰æ‚Ì•K—v‚È‚µ
}

void CameraCollider::Release(void)
{
}

void CameraCollider::HitCollider(std::weak_ptr<Collider> _col)
{
}

void CameraCollider::DoInit(void)
{
	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, TAG::OBJECT, std::move(std::make_unique<Sphere>(camera_.GetPos(), SPHERE_RADIUS)));
}

void CameraCollider::DoUpdate(void)
{
}
