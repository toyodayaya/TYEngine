#pragma once
#include <memory>
#include <vector>
#include "Sprite.h"

class ScoreManager
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class ScoreManager;
	};

	// PassKeyを受け取るコンストラクタ
	explicit ScoreManager(ConstructorKey) {}

private:
	// デストラクタ
	~ScoreManager() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;
	// インスタンス
	friend std::default_delete<ScoreManager>;
	static std::unique_ptr<ScoreManager> instance;

	// ダメージの桁数上限
	static const int kNumberArray = 4;
	// 数字用のスプライト
	std::vector<std::unique_ptr<Sprite>> numbers_;
	// スコアのスプライト
	std::unique_ptr<Sprite> scoreSprite_;
	// スコア
	int score_ = 0;

public:
	// インスタンス
	static ScoreManager* GetInstance();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();

	// スコア加算処理
	void AddScore(const int& score);

	// 数字をセットする関数
	void SetNumber();

};

