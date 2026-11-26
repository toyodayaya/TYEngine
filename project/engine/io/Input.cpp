#include "Input.h"
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
#include <cassert>

std::unique_ptr<Input> Input::instance = nullptr;

Input* Input::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<Input>(ConstructorKey());
	}
	return instance.get();
}

void Input::Initialize(WinAPIManager* winApiManager)
{
	// 借りてきたWinAPIのインスタンスを記録
	this->winApiManager_ = winApiManager;
	
	HRESULT inputResult = DirectInput8Create(
		winApiManager_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(inputResult));
	
	inputResult = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(inputResult));
	// 入力データ形式のセット
	inputResult = keyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(inputResult));
	// 排他制御レベルのセット
	inputResult = keyboard->SetCooperativeLevel(
		winApiManager_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(inputResult));
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(preKey, key, sizeof(key));

	// キーボード情報の取得開始
	keyboard->Acquire();
	
	keyboard->GetDeviceState(sizeof(key), key);
	
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーを押していたらtrueを返す
	if (key[keyNumber])
	{
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーをトリガーしていたらtrueを返す
	if (!preKey[keyNumber] && key[keyNumber])
	{
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::ReleaseKey(BYTE keyNumber)
{
	// 指定キーをリリースしていたらtrueを返す
	if (preKey[keyNumber] && !key[keyNumber])
	{
		return true;
	}

	// そうでなければfalseを返す
	return false;
}

bool Input::GetConnectController()
{
	DWORD dwResult;

	// コントローラーの状態を取得
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// ０番目のコントローラーの接続状況を取得
	dwResult = XInputGetState(0, &state);

	// コントローラーの接続状況を返す
	if (dwResult == ERROR_SUCCESS)
	{
		// 接続されている
		return true;
	}
	else
	{
		// 接続されていない
		return false;
	}

	return false;
}

Vector2 Input::GetLStickValue()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// ０番目のコントローラーの接続状況を取得
	DWORD dwResult = XInputGetState(0, &state);

	// コントローラーが接続されていない場合は入力なしとする
	if (dwResult != ERROR_SUCCESS)
	{
		return Vector2{ 0.0f,0.0f };
	}

	// Lスティックの入力を取得
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	// コントローラーがどれだけ押し込まれているか判定
	float magnitude = sqrt(LX * LX + LY * LY);

	// コントローラーが押された方向を記録する変数
	float normalizedLX = 0.0f;
	float normalizedLY = 0.0f;

	float normalizedMagnitude = 0.0f;

	// デッドゾーン内かどうか判定
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		// 最大値を超えていたら規制
		if (magnitude > 32767)
		{
			magnitude = 32767;
		}

		// スティックの振幅を調整
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		// 基準値に基づいて正規化する
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		// コントローラーが押された方向を判定
		normalizedLX = LX / magnitude;
		normalizedLY = LY / magnitude;
	}
	else
	{
		// デッドゾーン内の場合は振幅をゼロにする
		magnitude = 0.0f;
		normalizedMagnitude = 0.0f;
	}

	// Lスティックの入力値を返す
	Vector2 ret = { normalizedLX,normalizedLY };
	return ret;
}

bool Input::GetLStickRight()
{
	// Lスティックの入力値を取得
	float lStick = GetLStickValue().x;

	// 入力がない場合
	if (lStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以上の場合は右入力あり
		if (lStick > 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool Input::GetLStickLeft()
{
	// Lスティックの入力値を取得
	float lStick = GetLStickValue().x;

	// 入力がない場合
	if (lStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以下の場合は左入力あり
		if (lStick < 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool Input::GetLStickUp()
{
	// Lスティックの入力値を取得
	float lStick = GetLStickValue().y;

	// 入力がない場合
	if (lStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以上の場合は上入力あり
		if (lStick > 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool Input::GetLStickDown()
{
	// Lスティックの入力値を取得
	float lStick = GetLStickValue().y;

	// 入力がない場合
	if (lStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以下の場合は下入力あり
		if (lStick < 0.0f)
		{
			return true;
		}
	}

	return false;
}

Vector2 Input::GetRStickValue()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// ０番目のコントローラーの接続状況を取得
	DWORD dwResult = XInputGetState(0, &state);

	// コントローラーが接続されていない場合は入力なしとする
	if (dwResult != ERROR_SUCCESS)
	{
		return Vector2{ 0.0f,0.0f };
	}

	// Lスティックの入力を取得
	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;

	// コントローラーがどれだけ押し込まれているか判定
	float magnitude = sqrt(RX * RX + RY * RY);

	// コントローラーが押された方向を記録する変数
	float normalizedRX = 0.0f;
	float normalizedRY = 0.0f;

	float normalizedMagnitude = 0.0f;

	// デッドゾーン内かどうか判定
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		// 最大値を超えていたら規制
		if (magnitude > 32767)
		{
			magnitude = 32767;
		}

		// スティックの振幅を調整
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		// 基準値に基づいて正規化する
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		// コントローラーが押された方向を判定
		normalizedRX = RX / magnitude;
		normalizedRY = RY / magnitude;
	}
	else
	{
		// デッドゾーン内の場合は振幅をゼロにする
		magnitude = 0.0f;
		normalizedMagnitude = 0.0f;
	}

	// Rスティックの入力値を返す
	Vector2 ret = { normalizedRX,normalizedRY };
	return ret;
}

bool Input::GetRStickRight()
{
	// Rスティックの入力値を取得
	float rStick = GetRStickValue().x;

	// 入力がない場合
	if (rStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以上の場合は右入力あり
		if (rStick > 0.0f)
		{
			return true;
		}
	}
}

bool Input::GetRStickLeft()
{
	// Rスティックの入力値を取得
	float rStick = GetRStickValue().x;

	// 入力がない場合
	if (rStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以下の場合は左入力あり
		if (rStick < 0.0f)
		{
			return true;
		}
	}
}

bool Input::GetRStickUp()
{
	// Rスティックの入力値を取得
	float rStick = GetRStickValue().y;

	// 入力がない場合
	if (rStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以上の場合は上入力あり
		if (rStick > 0.0f)
		{
			return true;
		}
	}
}

bool Input::GetRStickDown()
{
	// Rスティックの入力値を取得
	float rStick = GetRStickValue().y;

	// 入力がない場合
	if (rStick == 0.0f)
	{
		return false;
	}
	else
	{
		// 入力値が0以下の場合は下入力あり
		if (rStick < 0.0f)
		{
			return true;
		}
	}
}

bool Input::GetStickRight()
{
	// どちらかのスティックで入力があればtrueを返す
	if (GetLStickRight() || GetRStickRight())
	{
		return true;
	}
	
	// なければfalseを返す
	return false;
}

bool Input::GetStickLeft()
{
	// どちらかのスティックで入力があればtrueを返す
	if (GetLStickLeft() || GetRStickLeft())
	{
		return true;
	}

	// なければfalseを返す
	return false;
}

bool Input::GetStickUp()
{
	// どちらかのスティックで入力があればtrueを返す
	if (GetLStickUp() || GetRStickUp())
	{
		return true;
	}

	// なければfalseを返す
	return false;
}

bool Input::GetStickDown()
{
	// どちらかのスティックで入力があればtrueを返す
	if (GetLStickDown() || GetRStickDown())
	{
		return true;
	}

	// なければfalseを返す
	return false;
}

bool Input::GetDpadUp()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 十字キーの上入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_UP)
	{
		return true;
	}

	return false;

}

bool Input::GetDpadDown()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 十字キーの下入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_DOWN)
	{
		return true;
	}

	return false;
}

bool Input::GetDpadRight()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 十字キーの右入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		return true;
	}

	return false;
}

bool Input::GetDpadLeft()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 十字キーの左入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_DPAD_LEFT)
	{
		return true;
	}

	return false;
}

bool Input::GetLbButton()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// LBボタンの入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		return true;
	}

	return false;
}

bool Input::GetRbButton()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// RBボタンの入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		return true;
	}

	return false;
}

bool Input::GetBButton()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// Bボタンの入力があるか
	if (state.Gamepad.wButtons && XINPUT_GAMEPAD_B)
	{
		return true;
	}

	return false;
}

void Input::Finalize()
{
	instance.reset();
}