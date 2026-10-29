#include "GamePlayScene.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "StageData.h"
#include "SkyboxCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "RenderTexture.h"
#include "PlayerManager.h"

void GamePlayScene::Initialize()
{
	// ポストエフェクトを指定
	RenderTexture::GetInstance()->SetMaxRadius(kMaxRadius_);
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("resources/enemy", "enemy.obj", Model::AnimationType::kNone);
	ModelManager::GetInstance()->LoadModel("resources/cube", "cube.obj", Model::AnimationType::kNone);
	ModelManager::GetInstance()->LoadModel("resources/skydome", "skydome.obj", Model::AnimationType::kNone);
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("resources/model/rostock_laage_airport_4k.dds");

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages", "1.json");
	StageManager::GetInstance()->LoadJsonData("resources/stages", "2.json");
	// マネージャーからステージ名を取得
	stageName_ = StageManager::GetInstance()->GetStageName();
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData(stageName_);
	// ステージを作成する
	stageData_->CreateStage(stageName_);
}

void GamePlayScene::Finalize()
{
	stageData_->ClearStage();
	stageData_ = nullptr;
}

void GamePlayScene::Update()
{
	// ステージを更新
	stageData_->Update();

	// 全ての当たり判定を走査
	stageData_->CheckAllCollision();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();
}

void GamePlayScene::Draw()
{
	// ステージを描画
	stageData_->Draw();
}
