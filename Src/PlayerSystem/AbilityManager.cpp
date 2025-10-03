#include"../Application.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Decoration/UIManager2d.h"
#include"../Object/Stage/StageManager.h"
#include"../Utility/Utility.h"
#include "AbilityManager.h"

const std::string AbilityManager::UI_ABILITY_MGNET = "MagnetIcon";
const std::string AbilityManager::UI_ABILITY_LOCK_TIME = "LockTimeIcon";
const VECTOR AbilityManager::ABILITY_ICON_POS = { 170.0f,230.0f,0.0f };

//ローカル定数
namespace {
	const FLOAT4 NONE_COLOR = { 0.0f,0.0f,0.0f,1.0f };
	const FLOAT4 MAGNET_COLOR = { 1.0f,0.0f,0.0f,1.0f };
	const FLOAT4 LOCK_TIME_COLOR = { 1.0f,1.0f,0.0f,1.0f };
	const FLOAT4 SELECT_COLOR = { 0.0,1.0f,0.0f ,1.0f};
	const VECTOR RETICLE_POS = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 ,0.0f };
	const float CAMERA_RAY_POW = 1000.0f;
	const float HIT_RETICLE_DIFF = 80.0f;
}

AbilityManager::AbilityManager(StageManager& _stage) :stage_(_stage)
{
	useAbility_ = ABILITY_TYPE::LOCK_TIME;
	isUsingAbility_ = false;

	auto& resM = ResourceManager::GetInstance();
	auto& uiM = UIManager2d::GetInstance();
	//マグネットアイコン
	uiM.Add(UI_ABILITY_MGNET, resM.Load(ResourceManager::SRC::ABILITY_MAGNET_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_MGNET, ABILITY_ICON_POS, UI_EX);

	//タイムロックアイコン
	uiM.Add(UI_ABILITY_LOCK_TIME, resM.Load(ResourceManager::SRC::ABILITY_LOCK_TIME_IMG).handleId_,
		UIManager2d::UI_DIRECTION_2D::NOMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(UI_ABILITY_LOCK_TIME, ABILITY_ICON_POS, UI_EX);

	iconNames_[static_cast<int>(ABILITY_TYPE::MAGNET)] = UI_ABILITY_MGNET;
	iconNames_[static_cast<int>(ABILITY_TYPE::LOCK_TIME)] = UI_ABILITY_LOCK_TIME;
}

AbilityManager::~AbilityManager(void)
{
}

void AbilityManager::Update(void)
{
	if (isUsingAbility_) {
		auto& camera = SceneManager::GetInstance().GetCamera();
		VECTOR cameraPos = camera.GetPos();
		VECTOR cameraRayEnd = VAdd(cameraPos, VScale(camera.GetRot().GetForward(), CAMERA_RAY_POW));
	
		//レティクルとの当たり判定
		for (auto& obj : stage_.GetAffectAbilityObjectes()) {
			//衝突していたら
			if (IsHitReticle(obj.lock()->GetScreenPos())) {
				//まだ参照するものがない場合
				if (selectObj_.expired()) {
					selectObj_ = obj;
				}
				else {
					//既に何かしら入っている場合
					//新しいオブジェクトの方が近い時
					if (!IsNearObject2Camera(selectObj_.lock()->GetPos(), obj.lock()->GetPos())) {
						//近いほうを採用
						selectObj_ = obj;
					}
				}
				//選択色の設定
				selectObj_.lock()->SetObjectRenderColor(SELECT_COLOR);
			}
			else {
				//当たり判定には判定していないがセレクトがあったとき
				if (!selectObj_.expired()) {
					//通常色の設定
					selectObj_.lock()->SetObjectRenderColor(NONE_COLOR);
				}
			}
		}
	}
}

void AbilityManager::Draw(void)
{
	//アイコンの描画
	if (useAbility_ != ABILITY_TYPE::NONE && useAbility_ != ABILITY_TYPE::MAX) {
		UIManager2d::GetInstance().Draw(iconNames_[static_cast<int>(useAbility_)]);
	}

	//能力使用時のレティクル
	if (isUsingAbility_) {
		DrawCircle(RETICLE_POS.x, RETICLE_POS.y, 10, 0x55ff00);
	}
}

void AbilityManager::RedyAbility(void)
{
	//使用中に
	isUsingAbility_ = true;

	//色の設定
	stage_.SetAbilityColor(GetAbilityColor(useAbility_));
}

void AbilityManager::UseAbility(void)
{
}

void AbilityManager::EndUsingAbility(void)
{
	//使用終了
	isUsingAbility_ = false;

	//付与色をなくす
	stage_.SetAbilityColor(NONE_COLOR);
}

void AbilityManager::ChangeAbility(const ABILITY_TYPE _type)
{
	useAbility_ = _type;
}

FLOAT4 AbilityManager::GetAbilityColor(const ABILITY_TYPE _type)
{
	FLOAT4 ret;

	if (_type == ABILITY_TYPE::MAGNET) {
		ret = MAGNET_COLOR;
	}
	else {
		ret = LOCK_TIME_COLOR;
	}

	return ret;
}

bool AbilityManager::IsHitReticle(VECTOR _screenPos)
{
	float diff = fabs(Utility::MagnitudeF(VSub(_screenPos, RETICLE_POS)));
	bool ret = false;
	if (diff <= HIT_RETICLE_DIFF) {
		ret = true;
	}
	return ret;
}

bool AbilityManager::IsNearObject2Camera(const VECTOR _pos1, const VECTOR _pos2)
{
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	float diff1 = Utility::MagnitudeF(VSub(_pos1, cameraPos));
	float diff2 = Utility::MagnitudeF(VSub(_pos2, cameraPos));
	return diff1 <= diff2;
}
