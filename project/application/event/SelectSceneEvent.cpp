#include "SelectSceneEvent.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "RenderTexture.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"
#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void SelectSceneEvent::Initialize(const QuaternionTransform& transform)
{
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

	// ステージのミニオブジェクトを読み込む
	ModelManager::GetInstance()->LoadModel("resources/stages/model", objectFilePath_, Model::AnimationType::kNone);
	// ステージのミニオブジェクトを初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetTransform(transform_);
	object3d_->SetModel(objectFilePath_);

	// ステージ名のスプライトを読み込む
	std::string filePath = "resources/stages/sprite/stage" + stageName_ + ".png";
	TextureManager::GetInstance()->LoadTexture(filePath);
	// ステージ名のスプライトを初期化
	stageNameSprite_ = std::make_unique<Sprite>();
	stageNameSprite_->Initialize(filePath);
	stageNameSprite_->SetPosition(Vector2{ 460.0f,60.0f });

#ifdef _DEBUG
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxRotate(transform.rotate);
	debugDraw->SetBoxTranslate(transform.translate);
#endif // _DEBUG
}

void SelectSceneEvent::Finalize()
{
#ifdef _DEBUG
	debugDraw.reset();
#endif // _DEBUG
}

void SelectSceneEvent::Update()
{
	// 3Dオブジェクトの更新
	object3d_->Update();
	// スプライトを更新する
	stageNameSprite_->Update();

	if (isHit_)
	{
		// 接触中にキー入力があったら
		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			// 次のステージ名を設定しロード
			StageManager::GetInstance()->SetStageName(stageName_);
			stageName_ += ".json";
			StageManager::GetInstance()->LoadJsonData("resources/stages/json", stageName_);

			// シーン遷移フラグを立てる
			isChangeScene_ = true;
			isHit_ = false;

			// フェードアウトフラグを立てる
			RenderTexture::GetInstance()->SetIsFadeIn(false);

			return;
		}

	}
	else if (isChangeScene_)
	{
		// シーン遷移フラグが立っていたら遷移演出

		// フェードアウト演出
		RenderTexture::GetInstance()->SceneChangeEffect();

		if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
		{
			// シーンを切り替える
			SceneManager::GetInstance()->ChangeScene("GamePlayScene");
			return;
		}
	}

	if (isHit_)
	{
		// 一旦フラグを解除
		isHit_ = false;
	}

#ifdef _DEBUG
	// デバッグ描画の更新処理
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void SelectSceneEvent::Draw()
{
	// 接触中のみスプライトを表示
	if (isHit_)
	{
		stageNameSprite_->Draw();
	}

	// モデルを描画
	object3d_->Draw();

#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void SelectSceneEvent::OnCollision(std::string hitObjectType)
{
	if (isChangeScene_)
	{
		// シーン遷移フラグが立っていたらスキップ
		return;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "PlayerSpawn")
	{
		// ヒットフラグを立てる
		isHit_ = true;
	}
}
