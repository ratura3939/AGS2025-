#pragma once
#include<string>
#include<vector>
#include<unordered_map>

class AnimationController
{
public:
	/// <summary>
	/// 再生種類
	/// </summary>
	enum class PLAY_TYPE {
		NOMAL,	//通常再生
		LOOP,	//ループ再生
		RETURN,	//逆再生
		MAX
	};

	//アニメーション関連情報
	struct AnimationInfo {
		PLAY_TYPE type;	//再生タイプ
		int idx;		//アニメーション番号
		float total;	//総再生時間
	};


	AnimationController(int& _model);
	~AnimationController(void);

	/// <summary>
	/// アニメーション追加関数
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_attach">アニメーション番号</param>
	/// <param name="_type">再生タイプ</param>
	void Add(const std::string& _name,const int _attach, const PLAY_TYPE _type);
	/// <summary>
	/// 再生開始処理
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_speed">再生速度</param>
	void Play(const std::string& _name,const float _speed);
	void Update(void);
private:
	//アニメーション終了時処理
	void FinishAnimNomal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

private:
	int& modelId_;	//モデルID
	std::unordered_map<std::string, AnimationInfo>animDatas_;	//アニメーションデータ総まとめ
	AnimationInfo activeAnim_;	//再生中のアニメーション情報
	int attachAnim_;
	float speedAnim;//再生速度
	float counter;	//更新カウンター

	using FinishAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;
};

