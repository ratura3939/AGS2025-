#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *instance_;
}

void InputManager::Init(void)
{

	
}

void InputManager::Update(void)
{
	//PAD関係は１Pの事しか見ていない
	//複数人を想定するのなら要改良
	//キーボード関係とPad関係で分けるのがよさそう？

	lastInput_ = currentInput_;
	lastInptuPeri_ = currentInptuPeri_;
	
	//キーボード
	char keystate[KEY_ALL] = {};
	GetHitKeyStateAll(keystate);
	//マウス
	int mousestate = GetMouseInput();

	//パッド
	int padstate = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//アナログ
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//項目分回す
	for (const auto& keyvalue : inputTable_) {
		bool pressed = false;	//押されているかどうかのフラグ
		std::vector<PeripheralType> inputTypes = {};
		//中身の動的配列をfor文で回す(キーボード→PADの順で見ている)
		for (auto input : keyvalue.second) {
			//キーボードのとき
			if (input.type == PeripheralType::KEYBOARD) {
				//GetHitKeyで入力状態をえたkeystateからコードの場所に検索をかける
				//pressed = keystate[input.code];
				if (keystate[input.code] != 0) {
					pressed = keystate[input.code];
					//入力が行われていたらこの危機から入力があったと記録する
					if (pressed)inputTypes.push_back(PeripheralType::KEYBOARD);
				}
			}
			else if (input.type == PeripheralType::GAMEPAD) {
				//パッドに何かしらの入力がありそれがコードだったとき
				pressed = padstate & input.code;
				if (pressed)inputTypes.push_back(PeripheralType::GAMEPAD);
			}
			else if (input.type == PeripheralType::MOUSE) {
				//パッドに何かしらの入力がありそれがコードだったとき
				pressed = mousestate & input.code;
				if (pressed)inputTypes.push_back(PeripheralType::MOUSE);
			}
			else if (input.type == PeripheralType::X_ANALOG) {
				pressed = analpgInputTable_[static_cast<AnalogInputType>(input.code)](xinputState);
				if (pressed)inputTypes.push_back(PeripheralType::X_ANALOG);
			}
		}
		currentInput_[keyvalue.first] = pressed;
		currentInptuPeri_[keyvalue.first] = inputTypes;
	}
}

void InputManager::Destroy(void)
{
	delete instance_;
}

void InputManager::ResetInput(void)
{
	// ゲームで使用したいキーとその名前を、
	// 事前にここで登録しておいてください

	//移動関係<WASD・左スティック>
	inputTable_["up"] = { { PeripheralType::KEYBOARD,KEY_INPUT_W },{ PeripheralType::GAMEPAD,PAD_INPUT_UP } };
	inputTable_["down"] = { { PeripheralType::KEYBOARD,KEY_INPUT_S },{ PeripheralType::GAMEPAD,PAD_INPUT_DOWN } };
	inputTable_["left"] = { { PeripheralType::KEYBOARD,KEY_INPUT_A },{ PeripheralType::GAMEPAD,PAD_INPUT_LEFT } };
	inputTable_["right"] = { { PeripheralType::KEYBOARD,KEY_INPUT_D },{ PeripheralType::GAMEPAD,PAD_INPUT_RIGHT } };
	//移動入力(サブ)<Rスティック・方向キー>
	inputTable_["upSub"] = { { PeripheralType::KEYBOARD,KEY_INPUT_UP },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::RS_UP) } };
	inputTable_["downSub"] = { { PeripheralType::KEYBOARD,KEY_INPUT_DOWN },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::RS_DOWN) } };
	inputTable_["leftSub"] = { { PeripheralType::KEYBOARD,KEY_INPUT_LEFT },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::RS_LEFT) } };
	inputTable_["rightSub"] = { { PeripheralType::KEYBOARD,KEY_INPUT_RIGHT },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::RS_RIGHT) } };

	//各コマンド<PADは複数個所で兼用あり>
	inputTable_["action"] = { { PeripheralType::MOUSE,MOUSE_INPUT_LEFT },{ PeripheralType::GAMEPAD,PAD_INPUT_B } };		//Bボタン(Aボタン：任天堂)
	inputTable_["dash"] = { { PeripheralType::KEYBOARD,KEY_INPUT_LSHIFT },{ PeripheralType::GAMEPAD,PAD_INPUT_A } };	//Aボタン(Bボタン：任天堂)
	inputTable_["cancel"] = { { PeripheralType::KEYBOARD,KEY_INPUT_Q },{ PeripheralType::GAMEPAD,PAD_INPUT_A } };		//Aボタン(Bボタン：任天堂)
	inputTable_["attack"] = { { PeripheralType::KEYBOARD,KEY_INPUT_W },{ PeripheralType::GAMEPAD,PAD_INPUT_C } };		//Xボタン(Yボタン：任天堂)
	inputTable_["jump"] = { { PeripheralType::KEYBOARD,KEY_INPUT_SPACE },{ PeripheralType::GAMEPAD,PAD_INPUT_X } };		//Yボタン(Xボタン：任天堂)
	inputTable_["crouch"] = { { PeripheralType::KEYBOARD,KEY_INPUT_LCONTROL },{ PeripheralType::GAMEPAD,PAD_INPUT_START } };//LS
	inputTable_["rock"] = { { PeripheralType::KEYBOARD,KEY_INPUT_R },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::LT) } };//LT
	inputTable_["arrow"] = { { PeripheralType::MOUSE,MOUSE_INPUT_RIGHT },{ PeripheralType::X_ANALOG,static_cast<int>(AnalogInputType::RT) } };//RT

	//ポーズ
	inputTable_["pause"] = { { PeripheralType::KEYBOARD,KEY_INPUT_TAB },{ PeripheralType::GAMEPAD,PAD_INPUT_R } };
}

void InputManager::AnalogInputFuncInit(void)
{
	analpgInputTable_[AnalogInputType::LS_UP] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLY > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::LS_DOWN] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLY < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::LS_RIGHT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLX > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::LS_LEFT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbLX < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::RS_UP] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRY > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::RS_DOWN] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRY < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::RS_RIGHT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRX > ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::RS_LEFT] = [](const XINPUT_STATE& _state) {
		return _state.ThumbRX < -ANALOG_STHICK_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::LT] = [](const XINPUT_STATE& _state) {
		return _state.LeftTrigger > ANALOG_TRIGGER_THRESHOLD;
	};
	analpgInputTable_[AnalogInputType::RT] = [](const XINPUT_STATE& _state) {
		return _state.RightTrigger > ANALOG_TRIGGER_THRESHOLD;
	};
}


bool InputManager::IsTrigerred(const std::string& _eventCode) const
{
	//先に要素がない場合の予防線をはる
	// 反応しないだけという状態を作りたいから
	//containd()=引数がキーとなる要素がないとき
	if (!currentInput_.contains(_eventCode)) {
		//参照できないので
		return false;
	}

	//[]の形で中身を見ようとすると勝手に中身が空のキーの場所が生成されてしまう
	//なのでmap型のat()関数はキー検索であり読み取り専用を使用することで中身を変えずに参照が可能になるｂ
	return currentInput_.at(_eventCode) && !lastInput_.at(_eventCode);
}

bool InputManager::IsPressed(const std::string& _eventCode) const
{
	return currentInput_.at(_eventCode);
}


