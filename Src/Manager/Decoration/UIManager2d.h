#pragma once
#include<string>
#include<vector>
#include<map>
#include<DxLib.h>

class UIManager2d
{
public:
	//UIの演出
	enum class UI_EFFECT_2D {
		NOMAL,		//通常
		MOVE_UP,	//上移動
		MOVE_DOWN,	//下移動
		MOVE_LEFT,	//左移動
		MOVE_RIGHT,	//右移動
		UP_DOWN,	//上下移動
		LEFT_RIGHT,	//左右移動
		ZOOM_IN,	//拡大
		ZOOM_OUT,	//縮小
		ZOOM_INOUT,	//拡大縮小
		ROT_RIGHT,	//右回転
		ROT_LEFT,	//左回転
		ROT_CRADLE,	//左右に回転
		GRAD_DISAP,	//徐々に消える
		GRAD_AP,	//徐々に現れる
		FLASHING,	//点滅		
	};

	struct UIInfo {
		VECTOR pos;	//位置
		float scl;	//大きさ
		float deg;	//角度
		float alpha;//透明度

		float speed;	//移動速度
		float rotPow;	//回転速度
		float alphaAcc;	//透明度増加量
	};

	static constexpr float ALPHA_MAX = 255.0f;
	static constexpr float ALPHA_MIN = 0.0f;

	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static UIManager2d& GetInstance(void);

	/// <summary>
	/// 描画情報の登録
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_imgHndl">描画する画像</param>
	/// <param name="_type">演出</param>
	void Add(const std::string& _name, const int _imgHndl, UI_EFFECT_2D _type);

	/// <summary>
	/// 演出追加
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_type">追加する演出</param>
	void AddOption(const std::string& _name, UI_EFFECT_2D _type);

	//更新
	void Update(const std::string _name);				//単体更新
	void Update(const std::vector<std::string> _names);	//複数更新

	//描画
	void Draw(const std::string _name);					//単体描画
	void Draw(const std::vector<std::string> _names);	//複数描画

private:
	//インスタンス用
	static UIManager2d* instance_;

	std::map<std::string, int>images_;	//描画画像格納
	std::map<std::string, UIInfo>infoes_;	//UIに関わる情報


	using Update_UI = void(UIManager2d::*)(const std::string);
	std::map<std::string, Update_UI>updates_;	//UI更新処理

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="_name">登録名</param>
	void Move(const std::string _name);

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="_name">登録名</param>
	void Rotation(const std::string _name);

	/// <summary>
	/// アルファ値調整
	/// </summary>
	/// <param name="_name">登録名</param>
	void AlphaAcc(const std::string _name);
};

