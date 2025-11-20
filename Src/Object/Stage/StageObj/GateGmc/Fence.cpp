#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/ResourceManager.h"
#include"../../../../Manager/GameSystem/CollisionManager.h"
#include"../../../../Renderer/ModelMaterial.h"
#include"../../../../Renderer/ModelRenderer.h"
#include"../../../Common/Geometry/Model.h"
#include"../../../Common/Geometry/Cube.h"
#include "Fence.h"

//ローカル定数1
namespace {
	const float UV_SCALING_NOISE = 10.0f;
	const std::string OBJ_NAME = "Gate";
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };
	const VECTOR INIT_SCL = { 0.4f,0.4f,0.4f };
	const float MOVE_SPEED = 6.0f;

	const VECTOR COLLIDER_SIZE = VECTOR{ 300.0f,600.0f,100.0f };
}

Fence::Fence(const VECTOR& _pos, const Quaternion& _rot)
{
	pos_ = _pos;
}

Fence::~Fence(void)
{
}

void Fence::Draw(void)
{
	//経過時間
	material_->SetConstBufPS(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });
	//描画
	render_->Draw();

	collider_->DrawDebugCollider();
}

void Fence::HitCollider(std::weak_ptr<Collider> _col)
{
}

void Fence::MoveFnece(const MOVE_DIR _dir)
{
	if(_dir == MOVE_DIR::UP){
		pos_.y += MOVE_SPEED;
		movedFenceDiff_ += MOVE_SPEED;
	}
	else if(_dir == MOVE_DIR::DOWN){
		pos_.y -= MOVE_SPEED;
		movedFenceDiff_ -= MOVE_SPEED;
	}
}

void Fence::SetParam(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.Load(ResourceManager::SRC::FENCE_MDL).handleId_;
	scl_ = INIT_SCL;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	//collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH}, std::move(std::make_unique<Model>(pos_, quaRot_, quaRotLocal_, scl_, modelId_)));
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::STAGE, COL_TYPE::SWITCH}, std::move(std::make_unique<Cube>(pos_, quaRot_, COLLIDER_SIZE)));


	CollisionManager::GetInstance().AddCollider(collider_);	//当たり判定登録

	//shader設定
	material_ = std::make_unique<ModelMaterial>("StdModelVS.cso", 0, "NoiseWavePS.cso", 3);
	//追加テクスチャ挿入
	material_->SetTextureBuf(ModelMaterial::SUB_TEX_1, resM.Load(ResourceManager::SRC::NOISE_STAGE).handleId_);
	//付与色
	material_->AddConstBufPS(NOMAL_COLOR);
	//経過時間
	material_->AddConstBufPS({ 0.0f,0.0f,0.0f,0.0f });
	//UV拡大率
	material_->AddConstBufPS({ UV_SCALING_NOISE,0.0f,0.0f,0.0f });

	isActiveGravity_ = false;

	//デバッグ
	isDrawScreenPosCircle_ = false;
	screenPosColor_ = 0xffff000;
}
