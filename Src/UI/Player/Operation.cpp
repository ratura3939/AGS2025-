#include"../../Application.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include "Operation.h"

//ローカル定数
namespace {
	//アクションボタン
	const std::string UI_OPERATION = "Operation";
	const VECTOR UI_OPERATION_POS = { 150.0f,500.0f,0.0f };
	const float UI_OPERATION_EX = 1.3f;

	//メニューボタン関連
	const std::string MENU_BTN = "MenuBtn";		//登録名
	const float BTN_EX = 0.6f;					//拡大率
	const int BTN_DIFF_X = 300;					//位置調整X
	const int BTN_DIFF_Y = 100;					//位置調整Y
	const VECTOR BTN_POSITION = { static_cast<float>(Application::SCREEN_SIZE_X - BTN_DIFF_X),static_cast<float>(Application::SCREEN_SIZE_Y - BTN_DIFF_Y),0.0f };

	//メニュー遷移キー
	const std::string UI_TO_MENU = "MenuOperation";
	const float UI_TO_MENU_EX = 0.5f;
	const VECTOR TO_MENU_POSITION_DIFF = { 150.0f,0.0f,0.0f };

	//アビリティ使用キー
	const std::string UI_ABILITY_OPERATION = "AbilityOperation";
	const float UI_ABILITY_OPERATION_EX = 0.8f;
	const VECTOR ABILITY_OPERATION_POSITION = { 300.0f,250.0f,0.0f };
}

Operation::Operation(VECTOR& _followPos):
	UIBase(_followPos)
	,buttonOperationStr_("")
	,menuOperationStr_("")
	,abilityOperationStr_("")
{
}

Operation::~Operation(void)
{
}

bool Operation::Init(const std::string& _master)
{
	ResourceManager& resM = ResourceManager::GetInstance();

    operationImgs_[static_cast<int>(SceneManager::CNTL::KEY)] = resM.Load(ResourceManager::SRC::KEY_OPERATION_IMG).handleId_;
    operationImgs_[static_cast<int>(SceneManager::CNTL::PAD)] = resM.Load(ResourceManager::SRC::PAD_OPERATION_IMG).handleId_;

	toMenuOperationImgs_[static_cast<int>(SceneManager::CNTL::KEY)] = resM.Load(ResourceManager::SRC::KEY_TO_MENU_OPERATION_IMG).handleId_;
	toMenuOperationImgs_[static_cast<int>(SceneManager::CNTL::PAD)] = resM.Load(ResourceManager::SRC::PAD_TO_MENU_OPERATION_IMG).handleId_;

	abilityOperationImgs_[static_cast<int>(SceneManager::CNTL::KEY)] = resM.Load(ResourceManager::SRC::KEY_ABILITY_OPERATION_IMG).handleId_;
	abilityOperationImgs_[static_cast<int>(SceneManager::CNTL::PAD)] = resM.Load(ResourceManager::SRC::PAD_ABILITY_OPERATION_IMG).handleId_;

	buttonOperationStr_ = _master + UI_OPERATION;
	menuOperationStr_ = _master + UI_TO_MENU;
	abilityOperationStr_ = _master + UI_ABILITY_OPERATION;

	UIManager2d& uiM = UIManager2d::GetInstance();

	//操作UIの設定
	uiM.Add(buttonOperationStr_, operationImgs_[static_cast<int>(SceneManager::CNTL::KEY)], UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(buttonOperationStr_, UI_OPERATION_POS, UI_OPERATION_EX);
	//操作UIの画像設定
	SetController(SceneManager::GetInstance().GetController());

	//メニューボタン
	uiM.Add(MENU_BTN, resM.Load(ResourceManager::SRC::MENU_BTN).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(MENU_BTN, BTN_POSITION, BTN_EX);

	//メニュー遷移ボタン
	uiM.Add(menuOperationStr_, toMenuOperationImgs_[static_cast<int>(SceneManager::CNTL::KEY)], UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(menuOperationStr_, VAdd(BTN_POSITION, TO_MENU_POSITION_DIFF), UI_TO_MENU_EX);

	//アビリティ使用ボタン
	uiM.Add(abilityOperationStr_, abilityOperationImgs_[static_cast<int>(SceneManager::CNTL::KEY)], UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	uiM.SetUIInfo(abilityOperationStr_, ABILITY_OPERATION_POSITION, UI_ABILITY_OPERATION_EX);

    return false;
}

bool Operation::Update(void)
{
    return false;
}

void Operation::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	//操作UIの描画
	uiM.Draw(buttonOperationStr_);

	//メニューボタンの表示
	uiM.Draw(MENU_BTN);

	//メニューへの遷移キーの表示
	uiM.Draw(menuOperationStr_);

	//アビリティ使用ボタンUIの表示
	uiM.Draw(abilityOperationStr_);
}

void Operation::Reset(void)
{
}

void Operation::SetController(const SceneManager::CNTL& _cntl)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	uiM.SetImage(buttonOperationStr_, operationImgs_[static_cast<int>(_cntl)]);
	uiM.SetImage(menuOperationStr_, toMenuOperationImgs_[static_cast<int>(_cntl)]);
	uiM.SetImage(abilityOperationStr_, abilityOperationImgs_[static_cast<int>(_cntl)]);
}
