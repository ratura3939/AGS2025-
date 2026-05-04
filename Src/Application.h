#pragma once
#include <string>

class Application
{
public:
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1920;	//横
	static constexpr int SCREEN_SIZE_Y = 1080;	//縦

	static constexpr float FRAME_RATE = 1000.0f / 60.0f;	//フレームレート
	static constexpr float DEFAULT_FPS = 60.0f;				//デフォルトFPS

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_IMAGE;	//画像
	static const std::string PATH_UI;		//UI
	static const std::string PATH_MODEL;	//モデル
	static const std::string PATH_CHARACTER;//キャラクターモデル
	static const std::string PATH_STAGE;	//ステージモデル
	static const std::string PATH_EFFECT;	//エフェクト
	static const std::string PATH_BGM;		//BGM
	static const std::string PATH_SE;		//SE
	static const std::string PATH_SHADER;	//シェーダー
	//-------------------------------------------

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	//スクリーンサイズ取得
	const int GetWindowWidth(void) { return width_; }
	const int GetWindowHeight(void) { return height_; }

private:
	//コンストラクタ・デストラクタ
	Application(void);
	Application(const Application&);
	~Application(void) = default;

	static Application* instance_;	// 静的インスタンス

	//フレーム固定用
	int currentFrame_;		//現在のフレームを保存
	int lastFrame_;			//最後に実行したフレームを保存

	bool isInitFail_;		//初期化失敗判定
	bool isReleaseFail_;	//解放失敗

	int width_;				//ウィンドウ横
	int height_;			//ウィンドウ縦
};