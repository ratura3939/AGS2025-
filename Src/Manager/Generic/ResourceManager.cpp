#include <DxLib.h>
#include<memory>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
}


void ResourceManager::InitTitle(void)
{
	Resource res;

	// スタートロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::START_LOGO, res);

	// デバイスアイコン
	ResorceDeviceIcon();

	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_ArrowDown.png");
	resourcesMap_.emplace(SRC::ARROW_DOWN_IMG, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Exit.png");
	resourcesMap_.emplace(SRC::EXIT_IMG, res);

	//音
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);
	//キャンセル
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SE, res);
	//カーソル動き
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "MoveCursur.mp3");
	resourcesMap_.emplace(SRC::MOVE_CURSUR_SE, res);
	//コントローラー選択
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "EnterController.mp3");
	resourcesMap_.emplace(SRC::ENTER_CNTL_SE, res);

}

void ResourceManager::InitGame(void)
{
	Resource res;

	//プレイヤー
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MDL, res);

	//敵
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + "Enemy_Axeman.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MDL, res);

	//ステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage1.mv1");
	resourcesMap_.emplace(SRC::OBJECTS_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "deco1.mv1");
	resourcesMap_.emplace(SRC::STAGE_MDL, res);

	//UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Question.png");
	resourcesMap_.emplace(SRC::SUSPECT_IMG, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Exclamation.png");
	resourcesMap_.emplace(SRC::FIND_IMG, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + "Icon_Heart.png");
	resourcesMap_.emplace(SRC::HEART_IMG, res);

	//音
	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game.mp3");
	resourcesMap_.emplace(SRC::GAME_BGM, res);
	//バトルBGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Battle.mp3");
	resourcesMap_.emplace(SRC::BATTLE_BGM, res);
	//歩き
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Walk.mp3");
	resourcesMap_.emplace(SRC::WALK_SE, res);
	//走り
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Run.mp3");
	resourcesMap_.emplace(SRC::RUN_SE, res);
	//剣振り
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "SwingSword.mp3");
	resourcesMap_.emplace(SRC::SWING_SWORD_SE, res);
	//ロックオン
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "RockOn.mp3");
	resourcesMap_.emplace(SRC::ROCK_ON_SE, res);
	//敵が発見！
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "FindPlayer.mp3");
	resourcesMap_.emplace(SRC::FIND_PLAYER_SE, res);
	//ダメージ
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + "Damage.mp3");
	resourcesMap_.emplace(SRC::DAMAGE_SE, res);

	//エフェクト
	//剣
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SwordEfc.efkefc");
	resourcesMap_.emplace(SRC::SWORD_EFC, res);
	//ダメージ
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFC, res);
}

void ResourceManager::InitClear(void)
{
	Resource res;

	// スタートロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameClear.png");
	resourcesMap_.emplace(SRC::GAMECLAR_LOGO, res);
}

void ResourceManager::InitGameOver(void)
{
	Resource res;
	// スタートロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER_LOGO, res);
}

void ResourceManager::ResorceDeviceIcon(void)
{
	Resource res;
	// PAD
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "IconPad.png");
	resourcesMap_.emplace(SRC::PAD_IMG, res);
	//Key
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "IconKeyboard.png");
	resourcesMap_.emplace(SRC::KEYBOARD_IMG, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
