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

/// <summary>
/// エフェクトの追加
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
/// <param name="_data">エフェクトのデータ</param>
void EffectManager::Add(const EFFECT& _efc, int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (effectRes_.find(_efc) != effectRes_.end())return;

	//新規データのため情報を追加
	effectRes_.emplace(_efc, _data);
}


/// <summary>
/// エフェクトの再生
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
/// <param name="_pos">再生位置</param>
/// <param name="_qua">角度</param>
/// <param name="_size">大きさ</param>
void EffectManager::Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const SoundManager::SOUND _sound)
{
	//元データがないときは警告
	if (effectRes_.find(_efc) == effectRes_.end())assert("設定していないエフェクトを再生しようとしています。");

	//再生配列内に要素が入っていないかを検索
	if (effectPlay_.find(_efc) == effectPlay_.end()) {
		//入っていないとき要素を追加する
		effectPlay_.emplace(_efc, PlayEffekseer3DEffect(effectRes_[_efc]));
	}else {
		//入っていたら元あるやつに上書きする
		effectPlay_[_efc] = PlayEffekseer3DEffect(effectRes_[_efc]);
	}

	//各種設定同期
	SyncEffect(_efc, _pos, _qua, _size);

	//効果音の再生
		if (_sound != SoundManager::SOUND::NONE) {
			SoundManager::GetInstance().Play(_sound);
		}
}


/// <summary>
/// エフェクトの再生停止
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
void EffectManager::Stop(const EFFECT& _efc)
{
	//配列内に入っていないものを停止しようとしたら警告
	if (effectPlay_.find(_efc) == effectPlay_.end())assert("設定していないエフェクトを停止しようとしています。");
	//再生停止
	StopEffekseer3DEffect(effectPlay_[_efc]);
}

void EffectManager::SyncEffect(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size)
{
	//その他各種設定
	//大きさ
	SetScalePlayingEffekseer3DEffect(effectPlay_[_efc], _size, _size, _size);
	//角度
	SetRotationPlayingEffekseer3DEffect(effectPlay_[_efc], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//位置
	SetPosPlayingEffekseer3DEffect(effectPlay_[_efc], _pos.x, _pos.y, _pos.z);
}

bool EffectManager::IsPlayEffect(const EFFECT& _efc)
{
	if (effectPlay_[_efc] == -1 || IsEffekseer3DEffectPlaying(effectPlay_[_efc]) == -1)
	{
		return true;
	}
	return false;
}

//解放処理
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
