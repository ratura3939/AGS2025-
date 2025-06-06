#pragma once
#include<string>
#include<vector>
#include<unordered_map>

//このWilidiaでは"idle"という名前のアニメーションを必ず入れること！
//NOMALのアニメーションは終了時idleを再生するようにするため


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
	void Play(const std::string& _name, const float _speed, const std::vector<std::string> _next = {});
	void Update(void);
private:
	//アニメーション更新処理
	void UpdateNomalAnim(void);
	void UpdateReturnAnim(void);

	//アニメーション終了時処理
	void FinishAnimNomal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

private:
	int& modelId_;	//モデルID
	std::unordered_map<std::string, AnimationInfo>animDatas_;	//アニメーションデータ総まとめ
	AnimationInfo activeAnim_;	//再生中のアニメーション情報
	int attachAnim_;//実際の再生しているもの
	float speedAnim;//再生速度
	float counter;	//更新カウンター

	std::vector<std::string> nextAnim_;	//次に再生するアニメーション(LOOP以外に適用)

	using FinishAnimation = void(AnimationController::*)(void);
	using UpdateAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;
	UpdateAnimation updateAnim_;
};

