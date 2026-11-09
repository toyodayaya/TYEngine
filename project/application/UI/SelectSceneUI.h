#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Sprite.h"

class SelectSceneUI
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class SelectSceneUI;
	};

	// PassKeyを受け取るコンストラクタ
	explicit SelectSceneUI(ConstructorKey) {}

	// シングルトンインスタンスの取得
	static SelectSceneUI* GetInstance();

	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();

	// カーソルを動かす処理
	void MoveCursor();
	// シーン切り替え処理
	void SceneChange();

private:
	// デストラクタ
	~SelectSceneUI() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	SelectSceneUI(const SelectSceneUI&) = delete;
	SelectSceneUI& operator=(const SelectSceneUI&) = delete;
	// インスタンス
	friend std::default_delete<SelectSceneUI>;
	static std::unique_ptr<SelectSceneUI> instance;

	// スプライト
	std::vector<std::unique_ptr<Sprite>> nextScene_;
	std::unique_ptr<Sprite> cursor_;
	int cursorNum_ = 0;

	// ステージ番号
	int stageNumber_ = 0;
	std::string stageName_;

};

