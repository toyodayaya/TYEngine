#include "GamePlayScene.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "StageData.h"
#include "ScoreManager.h"
#include "RenderTexture.h"
#include "ParticleManager.h"

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
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/trace.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/flash.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/bullet.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/beam.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/deadEnemy.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/particle/beamPoint.png");


	// パーティクルグループの作成
	ParticleManager::GetInstance()->CreateParticleGroup("Bullet", "resources/sprite/particle/bullet.png", ParticleManager::ShapeType::kNormal);
	ParticleManager::GetInstance()->CreateParticleGroup("Beam", "resources/sprite/particle/beam.png", ParticleManager::ShapeType::kNormal);
	ParticleManager::GetInstance()->CreateParticleGroup("Trace", "resources/sprite/particle/trace.png", ParticleManager::ShapeType::kNormal);
	ParticleManager::GetInstance()->CreateParticleGroup("Flash", "resources/sprite/particle/flash.png", ParticleManager::ShapeType::kNormal);
	ParticleManager::GetInstance()->CreateParticleGroup("DeadEnemy", "resources/sprite/particle/deadEnemy.png", ParticleManager::ShapeType::kNormal);
	ParticleManager::GetInstance()->CreateParticleGroup("BeamPoint", "resources/sprite/particle/beamPoint.png", ParticleManager::ShapeType::kNormal);

	// スコアをリセット
	ScoreManager::GetInstance()->ResetScore();

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
	// スコアUIを更新
	ScoreManager::GetInstance()->Update();

	// ステージを更新
	stageData_->Update();

	// 全ての当たり判定を走査
	stageData_->CheckAllCollision();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();

	// パーティクルの更新
	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	// ステージを描画
	stageData_->Draw();

	// スコアUIを描画
	ScoreManager::GetInstance()->Draw();

	// パーティクルの描画
	ParticleManager::GetInstance()->Draw();
}
