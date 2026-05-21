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

Board::Board(const VECTOR& _pos, const VECTOR& _size, const Quaternion& _qua)
{
	pos_ = _pos;
	scl_ = _size;
	quaRot_ = _qua;
}

Board::~Board(void)
{
}

void Board::DoHitCollider(const std::weak_ptr<Collider>& _col)
{
	
}

void Board::SetModel(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	modelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::WOOD_BOARD_MDL);

	speciesName_ = OBJ_NAME;

	//コライダー設定
	using COL_TYPE = Collider::COL_TAG;
	collider_ = std::make_shared<Collider>(*this, std::set<COL_TYPE>{COL_TYPE::OBJECT}, std::move(std::make_unique<Cube>(pos_, quaRot_,Utility::VMul(COLLIDER_SIZE,scl_))));
}
