#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/CollisionManager.h"
#include"../../../Renderer/ModelMaterial.h"
#include"../../../Utility/Utility.h"
#include"../../Common/Geometry/Cube.h"

#include"../../../Renderer/ModelRenderer.h"
#include "Board.h"

//ローカル定数
namespace {
	const std::string OBJ_NAME = "Board";
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };
	const VECTOR COLLIDER_SIZE = { 200.0f,20.0f,200.0f };
}

Board::Board(const VECTOR& _pos)
{
	pos_ = _pos;
}

Board::~Board(void)
{
}

void Board::DoHitCollider(const std::weak_ptr<Collider>& _col)
{
	//ステージタグが存在するとき
	if (_col.lock()->IsContainsTag(Collider::COL_TAG::STAGE) && !isAffectingNow_) {
		//SetPrevPos();

		VECTOR colNormal = _col.lock()->GetGeometry().GetHitNormal();				//法線ベクトル

		const float threshold = 0.3f;
		VECTOR enablePrevPos = Utility::EpsilonCustomThreshold(colNormal, threshold);

		VECTOR backPow = VSub(_col.lock()->GetGeometry().GetHitPoint(), pos_);

		if (enablePrevPos.x > 0.0f)pos_.x = prevPos_.x;
		if (enablePrevPos.y > 0.0f)pos_.y = prevPos_.y;
		if (enablePrevPos.z > 0.0f)pos_.z = prevPos_.z;

		//衝突した物体の法線方向に少し押し戻す
		moveDir_ = collider_->GetGeometry().GetHitNormal();
		moveSpeed_ = Utility::MagnitudeF(gravity_) * 0.5f;

		gravity_ = { 0.0f,0.0f,0.0f };
	}
}

void Board::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::WOOD_BOARD_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Cube>(pos_, quaRot_,Utility::VMul(COLLIDER_SIZE,scl_))));
}
