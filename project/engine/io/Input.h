#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <xinput.h>
#include "WinAPIManager.h"
#include "DirectXBasis.h"

// 入力クラス
class Input
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class Input;
	};

	// PassKeyを受け取るコンストラクタ
	explicit Input(ConstructorKey) {}

private:
	// デストラクタ
	~Input() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	// インスタンス
	friend std::default_delete<Input>;
	static std::unique_ptr<Input> instance;

public:
	// namespaceを省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// 初期化
	void Initialize(WinAPIManager* winApiManager);

	// 更新
	void Update();

	// キーのプッシュ判定を取得
	bool PushKey(BYTE keyNumber);

	// キーのトリガー判定を取得
	bool TriggerKey(BYTE keyNumber);

	// キーのリリース判定を取得
	bool ReleaseKey(BYTE keyNumber);

	// コントローラーの接続状況を取得
	bool GetConnectController();

	// Lスティックの入力値を取得
	Vector2 GetLStickValue();
	// Lスティックの右入力を取得
	bool GetLStickRight();
	// Lスティックの左入力を取得
	bool GetLStickLeft();
	// Lスティックの上入力を取得
	bool GetLStickUp();
	// Lスティックの下入力を取得
	bool GetLStickDown();

	// Rスティックの入力値を取得
	Vector2 GetRStickValue();
	// Rスティックの右入力を取得
	bool GetRStickRight();
	// Rスティックの左入力を取得
	bool GetRStickLeft();
	// Rスティックの上入力を取得
	bool GetRStickUp();
	// Rスティックの下入力を取得
	bool GetRStickDown();

	// 両スティックの右入力を取得
	bool GetStickRight();
	// 両スティックの左入力を取得
	bool GetStickLeft();
	// 両スティックの上入力を取得
	bool GetStickUp();
	// 両スティックの下入力を取得
	bool GetStickDown();

	// 十字キーの上入力を取得
	bool GetDpadUp();
	// 十字キーの下入力を取得
	bool GetDpadDown();
	// 十字キーの右入力を取得
	bool GetDpadRight();
	// 十字キーの左入力を取得
	bool GetDpadLeft();

	// LBボタンの入力を取得
	bool GetLbButton();
	// RBボタンの入力を取得
	bool GetRbButton();

	// Bボタンの入力を取得
	bool GetBButton();

	// インスタンス
	static Input* GetInstance();

	// 終了
	void Finalize();

private:
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard;
	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput;
	// 全キーの入力状態を取得する
	BYTE key[256] = {};
	// 全キーの前回の入力状態を取得する
	BYTE preKey[256] = {};
	// WindowsAPI
	WinAPIManager* winApiManager_ = nullptr;
};