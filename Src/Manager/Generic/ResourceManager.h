#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include"SceneManager.h"

class ResourceManager
{
public:
	// リソース名
	enum class SRC
	{
		//画像
		TITLE_LOGO
		,SHADOW_LOGO
		,PAD_IMG
		,KEYBOARD_IMG
		,GAMEOVER_LOGO
		,GAMECLEAR_LOGO
		,TITLE_BACK_BTN
		,BACK_GAME_BTN
		,MENU_BTN
		,START_GAME_BTN
		,STOP_GAME_BTN
		,CLICK_STR_IMG
		,BACK_BTN
		,SWITCH_BTN

		,DESCRIPTION_PAD
		,DESCRIPTION_KEY

		,CONFIRM_SWITCH
		,FINISH_SWITCH

		,FOCUS_IMG
		,NOISE_STAGE

		,RETICLE_IMG

		//UI
		,ARROW_DOWN_IMG
		,HEART_IMG
		,HEART_BROKEN_IMG
		,HEART_EMPTY_IMG
		,SUSPECT_IMG
		,FIND_IMG
		,EXIT_IMG
		,LOCKON_IMG
		,ANNOUNCE_LOCKON_IMG
		,PLATE_IMG
		,SKELETON_IMG
		,GOLEM_IMG
		,NUMBER_IMGS
		,WARNING_IMG
		,ATK_ALERT_IMG
		,RETRY_IMG
		,CHECK_CONFIG_IMG
		,SWITCH_OPERATOR_IMG
		,ABILITY_MAGNET_IMG
		,ABILITY_LOCK_TIME_IMG
		,PAD_OPERATION_IMG
		,KEY_OPERATION_IMG
		,PAD_TO_MENU_OPERATION_IMG
		,KEY_TO_MENU_OPERATION_IMG
		,PAD_ABILITY_OPERATION_IMG
		,KEY_ABILITY_OPERATION_IMG

		//モデル
		,STAGE_MDL
		,STAGE_GROUND_MDL
		,STAGE_WALL_MDL
		,WALL_SINGLE_MDL
		,INSIDE_WALL_MDL
		,OBJECTS_MDL
		,PLAYER_MDL
		,ENEMY_MDL
		,BOSS_MDL
		,SKY_DOME
		,SWITCH_MDL
		,GATE_MDL
		,FENCE_MDL
		,TURN_TABLE_MDL
		,CROSS_MDL
		,STAIRS_MDL
		,STAIRS_LOW_MDL
		,WOOD_BOARD_MDL
		,BLOCK_MDL
		,LIBRA_PLATE_MDL
		,MAGNET_LINE_MDL
		,TEST_MDL
		,TEST_MDL_2

		//音
		,TITLE_BGM
		,GAME_BGM
		,BATTLE_BGM
		,BOSS_BGM
		,WARNING_BGM
		,GAMECLEAR_BGM
		,GAMEOVER_BGM
		,CANCEL_SE
		,MOVE_CURSOR_SE
		,ENTER_CNTL_SE
		,WALK_SE
		,RUN_SE
		,SWING_SWORD_SE
		,LOCK_ON_SE
		,FIND_PLAYER_SE
		,DAMAGE_SE
		,BOSS_IMPACT_SE
		,BOSS_SHOUT_SE
		,ATK_ALERT_SE
		,DODGE_SE
		,JUST_DODGE_SE
		,ABILITY_SE
		,TIME_LOCK_SE
		,BREAK_TIME_LOCK_SE
		,CONNECT_MAGNET
		,WAVE_MAGNET_SE
		,FENCE_SE
		,SWITCH_SE
		,BOSS_DEATH_SE

		//エフェクト
		,SWORD_EFC
		,DAMAGE_EFC
		,CHARGE_ATK_EFC
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(SceneManager::SCENE_ID _scene= SceneManager::SCENE_ID::MAX);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);
	
private:
	//シーンごとにデータを読み込むことにする
	void InitTitle(void);		//タイトル
	void InitGame(void);		//ゲーム
	void InitClear(void);		//クリア
	void InitGameOver(void);	//ゲームオーバー
	void InitPouse(void);		//ポーズ
	void InitKeyConfig(void);	//キーコンフィグ
	void InitSwitchCntl(void);	//操作方法切り換え

	//大まかなリソースのまとまり
	void ResourceDeviceIcon(void);	//操作方法関連
	void ResourceStage(void);		//ステージ関連

	// 内部ロード
	Resource* _Load(SRC src);

	//コンストラクタ・デストラクタ
	ResourceManager(void);
	~ResourceManager(void) = default;

	static ResourceManager* instance_;		// 静的インスタンス
	std::map<SRC, Resource> resourcesMap_;	// リソース管理の対象
	std::map<SRC, Resource*> loadedMap_;	// 読み込み済みリソース
};

