#include"ActorBase.h"
#include "Collider.h"

Collider::Collider(ActorBase& _master, const std::set<MASTER_TYPE> _types, std::unique_ptr<Geometry> _geo, const std::set<MASTER_TYPE> _noHitTypes)
	: master_(_master)
	, geometry_(std::move(_geo))
	, types_(_types)
	, noHitTypes_(_noHitTypes)
{
}

Collider::~Collider(void)
{
}

void Collider::OnHit(Collider& _col)
{
	master_.HitCollider(_hitType);
}
