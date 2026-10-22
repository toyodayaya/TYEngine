#include "InGameUI.h"
#include "Player.h"
#include "TextureManager.h"
#include "MathManager.h"
using namespace MathManager;

void InGameUI::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	object3d_ = player_->GetObject3d();

	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/UI/move.png");
	TextureManager::GetInstance()->LoadTexture("resources/UI/hp.png");
	TextureManager::GetInstance()->LoadTexture("resources/UI/hpbar.png");
	TextureManager::GetInstance()->LoadTexture("resources/UI/enter.png");
	TextureManager::GetInstance()->LoadTexture("resources/UI/nakami.png");

	// UIスプライトの初期化
	moveSprite_ = std::make_unique<Sprite>();
	moveSprite_->Initialize("resources/UI/move.png");
	moveSprite_->SetPosition(Vector2{ 30.0f,520.0f });

	for (size_t i = 0; i < 3; i++)
	{
		auto hpSprite = std::make_unique<Sprite>();
		hpSprite_.push_back(std::move(hpSprite));

		auto beamSprite = std::make_unique<Sprite>();
		beamSprite_.push_back(std::move(beamSprite));
	}

	hpSprite_[0]->Initialize("resources/UI/hp.png");
	hpSprite_[0]->SetPosition(Vector2{ 1000.0f,550.0f });
	hpSprite_[1]->Initialize("resources/UI/hpbar.png");
	hpSprite_[1]->SetPosition(Vector2{ 1000.0f,600.0f });
	hpSprite_[2]->Initialize("resources/UI/nakami.png");
	hpSprite_[2]->SetPosition(Vector2{ 1010.0f,610.0f });

	beamSprite_[0]->Initialize("resources/UI/enter.png");
	beamSprite_[0]->SetPosition(Vector2{ 0.0f,0.0f });
	beamSprite_[1]->Initialize("resources/UI/hpbar.png");
	beamSprite_[1]->SetPosition(Vector2{ 0.0f,0.0f });
	beamSprite_[2]->Initialize("resources/UI/nakami.png");
	beamSprite_[2]->SetPosition(Vector2{ 0.0f,0.0f });

}

void InGameUI::Update()
{
	// 操作UIの更新
	moveSprite_->Update();

	// HPUIの更新
	for (const auto& hpSprite : hpSprite_)
	{
		hpSprite->Update();
	}


	// ポイントが貯まっていたら
	if (currentBeamPoint_ >= canFireBeamPoint)
	{
		// プレイヤーの位置を算出
		Vector3 playerpos = Project(Vector3{ 0.0f,0.0f,0.0f }, 0.0f, 0.0f, 1280.0f, 720.0f, object3d_->GetWorldViewProjection());
		std::vector<Vector2> pos = { {playerpos.x,playerpos.y},{playerpos.x,playerpos.y},{playerpos.x,playerpos.y} };
		pos[2].x += 10.0f;
		pos[2].y += 10.0f;

		// ビームUIを更新
		for (int i = 0; i < 3; i++)
		{
			beamSprite_[i]->SetPosition(pos[i]);
			beamSprite_[i]->Update();
		}
	}
}

void InGameUI::Draw()
{
	// 操作UIの描画
	moveSprite_->Draw();

	// HPUIの描画
	for (const auto& hpSprite : hpSprite_)
	{
		hpSprite->Draw();
	}

	// ポイントが貯まっていたら
	if (currentBeamPoint_ >= canFireBeamPoint)
	{
		// ビームUIを描画
		for (const auto& beamSprite : beamSprite_)
		{
			beamSprite->Draw();
		}
	}
}
