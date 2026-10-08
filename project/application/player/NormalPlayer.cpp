#include "NormalPlayer.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "BulletManager.h"
#include "SceneManager.h"
#include "PlayerManager.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void NormalPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	camera_ = player_->GetCamera();

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(true);
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.translate = Vector3Add(transform_.translate, offset_);
	isHit_ = false;

	// 3Dレティクルオブジェクトの初期化
	reticle_ = std::make_unique<Object3d>();
	reticle_->Initialize();
	reticle_->SetModel(filePath_);
	reticle_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	reticle_->SetTransform(transform_);
	reticle_->SetIsRailCamera(true);
	reticleTransform_ = transform_;
	reticleTransform_.translate = Vector3Add(reticleTransform_.translate, offset_);

	// ロックオンマークを初期化
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
}

void NormalPlayer::Update()
{
	// キー入力で弾を生成
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		CreateBullet();
	}

	// プレイヤーの移動処理
	Move();

	// 3Dレティクルを更新
	UpdateReticle();

	// ロックオンマークを更新
	lockOn_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &transform_.translate.x);
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::DragFloat3("target", &targetPosition.x);

	ImGui::End();

#endif // USE_IMGUI


}

void NormalPlayer::Draw()
{
	// 当たっていたら非表示にする
	if (isHit_)
	{
		return;
	}

	object3d_->Draw();
	lockOn_->Draw();
}

void NormalPlayer::Finalize()
{}

void NormalPlayer::OnCollision()
{
	// ステートを変更する
	PlayerManager::GetInstance()->ChangePlayerState("DeadPlayer");
}

void NormalPlayer::CreateBullet()
{

	if (lockOn_->GetTarget())
	{
		targetPosition = lockOn_->GetTarget()->GetWorldTranslate();
	}
	else
	{
		targetPosition = GetReticleWorldTranslate();
	}

	// 弾の初期座標を設定
	QuaternionTransform bulletTransform = transform_;
	bulletTransform.translate = GetWorldTranslate();

	// 速度を算出
	velocity = Vector3Subtract(targetPosition, bulletTransform.translate);
	velocity = Normalize(velocity);
	velocity = FloatMultiply(velocity, kBulletSpeed_);

	// 生成と初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize(bulletTransform, filePath_, velocity, false);
	BulletManager::GetInstance()->SetBullets(std::move(bullet));

}

void NormalPlayer::UpdateReticle()
{
	// 自機のワールド行列の回転を適用
	Matrix4x4 world = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	offset_ = TransformNormal(offset_, world);
	// ベクトルの長さを整える
	offset_ = FloatMultiply(Normalize(offset_), kDistance_);
	// 3Dレティクルの位置を決定
	reticleTransform_.translate = Vector3Add(transform_.translate, offset_);
	reticle_->SetTransform(reticleTransform_);

	// 3Dオブジェクトの更新
	reticleWorldMatrix = MakeAffineMatrixQuat(reticleTransform_.scale, reticleTransform_.rotate, reticleTransform_.translate);
	reticleWorldMatrix = Multiply(reticleWorldMatrix, camera_->GetWorldMatrix());
	reticle_->SetWorldMatrix(reticleWorldMatrix);
	reticle_->Update();

	// スプライトのレティクルに座標設定
	lockOn_->LockOnTarget(reticle_);

#ifdef USE_IMGUI
	ImGui::Begin("Reticle");
	ImGui::DragFloat3("pos", &reticleTransform_.translate.x);

	ImGui::End();

#endif // USE_IMGUI


}

void NormalPlayer::Move()
{
	// キー入力でプレイヤーを移動させる
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		transform_.translate.x -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_D))
	{
		transform_.translate.x += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_S))
	{
		transform_.translate.y -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_W))
	{
		transform_.translate.y += 0.1f;
	}

	// 範囲を超えない処理
	transform_.translate.x = max(transform_.translate.x, -kMoveLimitX_);
	transform_.translate.x = std::min(transform_.translate.x, +kMoveLimitX_);
	transform_.translate.y = max(transform_.translate.y, -kMoveLimitY_);
	transform_.translate.y = std::min(transform_.translate.y, +kMoveLimitY_);


	// 座標を更新
	object3d_->SetTranslate(transform_.translate);

	// プレイヤーのローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	// カメラのワールド行列を乗算
	worldMatrix = Multiply(worldMatrix, camera_->GetWorldMatrix());
	// プレイヤーのワールド座標をセット
	object3d_->SetWorldMatrix(worldMatrix);
	player_->SetWorldMatrix(worldMatrix);

	// 3Dオブジェクトを更新
	object3d_->Update();
}
