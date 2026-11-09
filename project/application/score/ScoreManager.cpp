#include "ScoreManager.h"
#include "TextureManager.h"
#include "MathManager.h"
using namespace MathManager;

std::unique_ptr<ScoreManager> ScoreManager::instance = nullptr;


ScoreManager* ScoreManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<ScoreManager>(ConstructorKey());
	}
	return instance.get();
}

void ScoreManager::Initialize()
{
	// 数字用の画像を読み込む
	TextureManager::GetInstance()->LoadTexture("resources/sprite/numbers/numbers.png");
	// スコア用の画像を読み込む
	TextureManager::GetInstance()->LoadTexture("resources/sprite/numbers/score.png");

	// スコア画像を初期化
	scoreSprite_ = std::make_unique<Sprite>();
	scoreSprite_->Initialize("resources/sprite/numbers/score.png");
	scoreSprite_->SetPosition(Vector2(650.0f, 10.0f));

	// 数字の画像を初期化
	for (size_t i = 0; i < kNumberArray; i++)
	{
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize("resources/sprite/numbers/numbers.png");
		sprite->SetPosition(Vector2{ 1000.0f + (50.0f * i),10.0f });
		numbers_.push_back(std::move(sprite));
	}

	// スコアを確定させる
	SetNumber();
}

void ScoreManager::Update()
{
	// スプライトを更新する
	scoreSprite_->Update();

	for (auto& sprite : numbers_)
	{
		sprite->Update();
	}
}

void ScoreManager::Draw()
{
	// スプライトを描画する
	scoreSprite_->Draw();

	for (auto& sprite : numbers_)
	{
		sprite->Draw();
	}
}

void ScoreManager::Finalize()
{
	instance.reset();
}

void ScoreManager::AddScore(const int& score)
{
	score_ += score;
}

void ScoreManager::SetNumber()
{
	// スコアが桁数を超えていたらカンスト
	if (score_ > 9999)
	{
		score_ = 9999;
	}

	// 現在の数値を記録
	int currentNumber = score_;

	// 1桁ずつ数字のUV座標を指定する
	for (int i = 3; i >= 0; i--)
	{
		// この桁の数字を算出
		int num = currentNumber % 10;
		// 数字の切り出し位置を指定
		numbers_[i]->SetTextureLeftTop(Vector2{ 100.0f * static_cast<float>(num), 0.0f });
		// 数字の切り出しサイズを指定
		numbers_[i]->SetTextureSize(Vector2{ 100.0f,100.0f });
		numbers_[i]->SetSize(Vector2{ 100.0f,100.0f });
		// 次の桁の数字を算出
		currentNumber /= 10;
	}


}
