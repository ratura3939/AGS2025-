#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;


class Title : public SceneBase
{

public:
	enum class DEVICE {
		KEY,
		PAD,
		MAX
	};

	static constexpr int SIZE_FONT = 32;	//フォントの大きさ
	static constexpr int THICK_FONT = 4;	//フォントの太さ

	//タイトルロゴ
	static constexpr float LOGO_ALL_DIFF = 450.0f;		//位置調整
	static constexpr float LOGO__ALL_MARGIN_Y = 200.0f;	//上方の隙間調整
	static constexpr float LOGO_NOMAL_EXTEND = 0.8f;			//拡大率
	static constexpr float LOGO_SHADOW_EXTEND = 0.4f;		//拡大率(影絵)
	static constexpr float LOGO_SHADOW_MARGIN_X = 40.0f;	//拡大率(影絵)

	//スタートボタン
	static constexpr float START_BTN_MARGIN_Y = 50.0f;
	static constexpr float START_BTN_EXTEND_MAX = 0.7f;	//拡大率(上限)
	static constexpr float START_BTN_EXTEND_MIN = 0.55f;	//拡大率(下限)
	static constexpr float START_BTN_EXTEND_ACC = 0.01f;//拡大率(加算)

	//「決定してください」
	static constexpr float ENTER_STR_EXTEND = 0.2f;

#pragma region コントローラー選択時
	//コントローラーアイコン
	static constexpr int DEVICE_SIZE = 300;			//サイズ(正方形)

	//矢印
	static constexpr int ALLOW_ICON_SIZE_X = 199;	//Xサイズ
	static constexpr int ALLOW_ICON_SIZE_Y = 288;	//Yサイズ
	//矢印の演出
	static constexpr float JUMP_POW_MAX = 0;	//動き幅(上限)
	static constexpr float JUMP_POW_MIN = -60;//動き幅(下限)
	static constexpr float JUMP_ACC = -5;	//矢印動き用

	//Exit
	static constexpr int EXIT_ICON_SIZE_X = 180;		//Xサイズ
	static constexpr int EXIT_ICON_SIZE_Y = 243;		//Yサイズ
	static constexpr float EXIT_EXTEND_MAX = 1.5f;	//拡大率(上限)
	static constexpr float EXIT_EXTEND_MIN = 0.6f;	//拡大率(下限)
	static constexpr float EXIT_EXTEND_ACC = 0.05f;	//拡大率(加算)

	//隙間調整用
	static constexpr int MARGIN_SIZE = 30;			//隙間の大きさ
	static constexpr float EXTEND_IMG = 1.5f;		//画像拡大率

#pragma endregion
	
	Title(void);
	~Title(void);

	void Init(void) override;
	void InitUI(void);
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	void NomalUpdate(void);
	void SelectDeviceUpdate(void);

	/// <summary>
	/// 使用するデバイスの選択設定
	/// </summary>
	/// <param name="_device">使用しようとしているデバイス</param>
	void SetSelectDevice(const DEVICE _device);

	//デバイス選択
	void DrawDevice(void);

	
	int backImg_;	//背景
	int deviceImgs_[static_cast<int>(DEVICE::MAX)];	//コントローラー画像

	bool isSelectDevice_;	//コントローラー選択中かどうか
	bool selectDevice_[static_cast<int>(DEVICE::MAX)];//選ばれているか
	bool selectExit_;	//「戻る」が選択されているかどうか

	VECTOR allowPos_[static_cast<int>(DEVICE::MAX)];

	using Update_f = void(Title::*)(void);
	Update_f update_;

	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;
};
