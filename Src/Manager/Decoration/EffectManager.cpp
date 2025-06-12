#include<EffekseerForDXLib.h>
#include<cassert>
#include "EffectManager.h"

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new EffectManager();
	}
}

EffectManager& EffectManager::GetInstance(void)
{
	return *instance_;
}

EffectManager::EffectManager(void) {
	int i[NONE_MAX] = {};

	//effectTest_.emplace(EFFECT::NONE,i);
}

void EffectManager::Add(const std::string& _name, int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (effectRes_.find(_name) != effectRes_.end())return;

	//新規データのため情報を追加
	effectRes_.emplace(_name, _data);
}

void EffectManager::Play(const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const std::string _sndName)
{
	//元データがないときは警告
	if (effectRes_.find(_name) == effectRes_.end())assert("設定していないエフェクトを再生しようとしています。");

	//再生配列内に要素が入っていないかを検索
	if (effectPlay_.find(_name) == effectPlay_.end()) {
		//入っていないとき要素を追加する
		effectPlay_.emplace(_name, PlayEffekseer3DEffect(effectRes_[_name]));
	}else {
		//入っていたら元あるやつに上書きする
		effectPlay_[_name] = PlayEffekseer3DEffect(effectRes_[_name]);
	}

	//各種設定同期
	SyncEffect(_name, _pos, _qua, _size);

	//効果音の再生
		if (_sndName != "") {
			SoundManager::GetInstance().Play(_sndName);
		}
}

void EffectManager::Stop(const std::string& _name)
{
	//配列内に入っていないものを停止しようとしたら警告
	if (effectPlay_.find(_name) == effectPlay_.end())assert("設定していないエフェクトを停止しようとしています。");
	//再生停止
	StopEffekseer3DEffect(effectPlay_[_name]);
}

void EffectManager::SyncEffect(const std::string& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size)
{
	//その他各種設定
	//大きさ
	SetScalePlayingEffekseer3DEffect(effectPlay_[_name], _size, _size, _size);
	//角度
	SetRotationPlayingEffekseer3DEffect(effectPlay_[_name], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//位置
	SetPosPlayingEffekseer3DEffect(effectPlay_[_name], _pos.x, _pos.y, _pos.z);
}

bool EffectManager::IsPlayEffect(const std::string& _name)
{
	if (effectPlay_[_name] == -1 || IsEffekseer3DEffectPlaying(effectPlay_[_name]) == -1)
	{
		return true;
	}
	return false;
}

void EffectManager::Release(void)
{
	//配列内の要素を全て消去
	//元々のデータはリソースマネージャが持っているのでおｋ。
	effectRes_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	delete instance_;
}
