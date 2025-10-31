#include"ActorBase.h"
#include "Collider.h"

Collider::Collider(ActorBase& _master, const std::set<COL_TAG> _tags, std::unique_ptr<Geometry> _geo, const std::set<COL_TAG> _noHitTags)
	: master_(_master)
	, geometry_(std::move(_geo))
	, tags_(_tags)
	, noHitTags_(_noHitTags)
{
}

Collider::~Collider(void)
{
}

void Collider::OnHit(std::weak_ptr<Collider> _col)
{
	master_.HitCollider(_col);
}

void Collider::DeleteNoHitTag(const COL_TAG& _tag)
{
	//タグが存在しなかったら処理しない
	if (!noHitTags_.contains(_tag))return;
	noHitTags_.erase(_tag);
}

void Collider::DeleteTag(const COL_TAG& _tag)
{
	//タグが存在しなかったら処理しない
	if (!tags_.contains(_tag))return;
	tags_.erase(_tag);
}

void Collider::DrawDebugCollider(void)
{
	geometry_->DebugDraw();
}
