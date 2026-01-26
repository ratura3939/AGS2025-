#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Object/Common/Geometry/Line.h"
#include"../../Utility/Utility.h"
#include "MagnetFallLine.h"

namespace {
	const float LINE_FALL_VEC_Y = 100000.0f;	//ラインのY方向の長さ
	const float MODEL_SIZE_Y = 200.0f;			//モデルのY方向の大きさ
}

MagnetFallLine::MagnetFallLine(void)
	: ActorBase()
	, material_(nullptr)
	, renderer_(nullptr)
	, nearFallPointY_(0.0f)
	, preNearFallPointY_(0.0f)
	, lineStartPos_(Utility::VECTOR_INIT)
	, lineEndPos_(Utility::VECTOR_INIT)
{
	isActiveGravity_ = false;
}

MagnetFallLine::~MagnetFallLine(void)
{
}

void MagnetFallLine::Draw(void)
{
	renderer_->Draw();

	collider_->GetGeometry().DebugDraw();

	VECTOR fallPoinSpherePos = pos_;
	fallPoinSpherePos.y = nearFallPointY_;
	DrawSphere3D(fallPoinSpherePos, 40, 8, 0x00ff00, 0x00ff00, false);
}

void MagnetFallLine::Release(void)
{
}

void MagnetFallLine::HitCollider(std::weak_ptr<Collider> _col)
{
	using TAG = Collider::COL_TAG;

	//オブジェクト・ステージタグとの当たり判定
	if (_col.lock()->IsContainsAnyTag(std::set<TAG>{TAG::OBJECT, TAG::STAGE})) {

		float colHitPointY = collider_->GetGeometry().GetHitPoint().y;

		//一番近い落下地点の更新
		if (nearFallPointY_ < colHitPointY || nearFallPointY_ == 0.0f) {
			nearFallPointY_ = colHitPointY;
		}
	}
}

void MagnetFallLine::DoInit(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();

	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::MAGNET_LINE_MDL);
	material_ = std::make_unique<ModelMaterial>("MagnetLineVS.cso", 0,"MagnetLinePS.cso", 1);

	material_->SetConstBufPS(0, FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f });

	quaRot_ = Quaternion::Euler(Utility::Deg2RadF(180.0f), 0.0f, 0.0f);

	//追加テクスチャ挿入
	material_->SetTextureBuf(ModelMaterial::SUB_TEX_1, resM.Load(ResourceManager::SRC::LOCKON_IMG).handleId_);

	renderer_ = std::make_unique<ModelRenderer>(modelId_, *material_);

	using TAG = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<TAG>{TAG::FALL_LINE}, std::make_unique<Line>(pos_,quaRot_,lineStartPos_,lineEndPos_), std::set<TAG>{TAG::PLAYER, TAG::ENEMY});
	CollisionManager::GetInstance().AddCollider(collider_);
}

void MagnetFallLine::DoUpdate(void)
{
	//線の始点・終点設定
	lineStartPos_ = pos_;
	lineStartPos_.y += MV1GetMeshMinPosition(modelId_, 0).y;
	lineEndPos_ = lineStartPos_;
	lineEndPos_.y -= LINE_FALL_VEC_Y;

	material_->SetConstBufPS(0, FLOAT4{ SceneManager::GetInstance().GetTotalTime(), 0.0f, 0.0f, 0.0f });;

	//落下地点が変わったか
	if (preNearFallPointY_ != nearFallPointY_) {
		ChangeSizeYToFallPoint();
	}

	//前回の落下地点保存
	preNearFallPointY_ = nearFallPointY_;
	nearFallPointY_ = 0.0f;
}

void MagnetFallLine::ChangeSizeYToFallPoint(void)
{
	//落下地点までの距離計算
	const float fallDistanceY = nearFallPointY_ - lineStartPos_.y;

	//スケール変更
	scl_.y = fallDistanceY / MODEL_SIZE_Y;
	MV1SetScale(modelId_, scl_);
}
