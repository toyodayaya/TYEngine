#include "SelectScenePlayer.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void SelectScenePlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	camera_ = player_->GetCamera();

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(true);
	transform_ = object3d_->GetTransform();
	transform_.translate = { 0.0f,-0.3f,0.0f };
	transform_.translate = Vector3Add(transform_.translate, playerOffset_);
	transform_.rotate = { 0.0f,0.1f,-0.02f,0.0f };
}

void SelectScenePlayer::Update()
{
	// プレイヤーの向きで分岐
	switch (direction_)
	{
	case kFront:

		// キー入力があったら
		if (Input::GetInstance()->TriggerKey(DIK_D))
		{
			// 目標角度を右に設定
			startAngle_ = transform_.rotate;
			targetAngle_ = transform_.rotate;
			targetAngle_.w = 0.1f;
			t = 0.0f;
			// 状態を回転中に設定
			direction_ = kTurn;
		}
		else if (Input::GetInstance()->TriggerKey(DIK_A))
		{
			// 目標角度を左に設定
			startAngle_ = transform_.rotate;
			targetAngle_ = transform_.rotate;
			targetAngle_.w = -0.1f;
			t = 0.0f;
			// 状態を回転中に設定
			direction_ = kTurn;
		}

		break;

	case kTurn:
		// 線形補間用の変数を加算
		t += 0.055f;

		// 向きを補間
		angle_ = Slerp(startAngle_, targetAngle_, t);
		transform_.rotate = angle_;

		// 向き切ったらリセット
		if (t >= 1.0f)
		{
			t = 0.0f;

			// 目標角度が左右どちらかだったら
			if (targetAngle_.w == -0.1f || targetAngle_.w == 0.1f)
			{
				// プレイヤーの向きの状態を左右に設定
				direction_ = kLeftAndRight;
			}
			else
			{
				// 正面に設定
				direction_ = kFront;
			}

		}
		break;

	case kLeftAndRight:

		break;
	}

	// プレイヤーのローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	// カメラのワールド行列を乗算
	worldMatrix = Multiply(worldMatrix, camera_->GetWorldMatrix());
	// プレイヤーのワールド座標をセット
	object3d_->SetWorldMatrix(worldMatrix);
	player_->SetWorldMatrix(worldMatrix);

	// 3Dオブジェクトを更新
	object3d_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &transform_.translate.x, 0.01f);
	ImGui::DragFloat4("rotate", &transform_.rotate.x, 0.01f);
	ImGui::End();

#endif // USE_IMGUI


}

void SelectScenePlayer::Draw()
{
	// オブジェクトを描画する
	object3d_->Draw();
}

void SelectScenePlayer::Finalize()
{}

void SelectScenePlayer::OnCollision(std::string hitObjectType)
{
	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "EventSpawn")
	{
		// プレイヤーの向きで分岐
		switch (direction_)
		{
		case kLeftAndRight:
			// 目標角度を正面に設定
			startAngle_ = transform_.rotate;
			targetAngle_ = { 0.0f,0.1f,-0.02f,0.0f };
			// 状態を回転中に変更
			direction_ = kTurn;
			break;
		}
	}
}