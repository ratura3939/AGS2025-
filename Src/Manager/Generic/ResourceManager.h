#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		//画像
		START_LOGO,
		PAD_IMG,
		KEYBOARD_IMG,
		GAMEOVER_LOGO,
		GAMECLAR_LOGO,
		//UI
		ARROW_DOWN_IMG,
		HEART_IMG,
		SUSPECT_IMG,
		FIND_IMG,
		EXIT_IMG,

		//モデル
		STAGE_MDL,
		OBJECTS_MDL,
		PLAYER_MDL,
		ENEMY_MDL,

		//音
		TITLE_BGM,
		GAME_BGM,
		BATTLE_BGM,

		CANCEL_SE,
		MOVE_CURSUR_SE,
		ENTER_CNTL_SE,
		WALK_SE,
		RUN_SE,
		SWING_SWORD_SE,
		ROCK_ON_SE,
		FIND_PLAYER_SE,
		DAMAGE_SE,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	//シーンごとにデータを読み込むことにする
	void InitTitle(void);
	void InitGame(void);
	void InitClear(void);
	void InitGameOver(void);
private:
	void ResorceDeviceIcon(void);

public:
	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

