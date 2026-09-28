#include "Player.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "BulletManager.h"
#include "SceneManager.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void Player::Initialize(const QuaternionTransform& transform, const std::string& filePath, bool isRailCamera)
{
	// オブジェクトの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());
	object3d_->SetModel(filePath);
	object3d_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	object3d_->SetTransform(transform);
	object3d_->SetIsRailCamera(isRailCamera);
	transform_ = transform;
	transform_.translate = Vector3Add(transform_.translate, offset_);
	isHit_ = false;


	// 3Dレティクルオブジェクトの初期化
	reticle_ = std::make_unique<Object3d>();
	reticle_->Initialize(Object3dCommon::GetInstance());
	reticle_->SetModel(filePath_);
	reticle_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	reticle_->SetTransform(transform_);
	reticle_->SetIsRailCamera(isRailCamera);
	reticleTransform_ = transform_;
	reticleTransform_.translate = Vector3Add(reticleTransform_.translate, offset_);

	// ロックオンマークを初期化
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();

	isDead_ = false;

#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);
	debugDraw->SetIsRailCamera(isRailCamera);

#endif // _DEBUG
}

void Player::Update()
{
	// キー入力で弾を生成
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		CreateBullet();
	}

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

	// 3Dオブジェクトを更新
	object3d_->Update();

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

#ifdef _DEBUG
	debugDraw->SetWorldMatrix(worldMatrix);
	debugDraw->UpdateBox();
#endif // _DEBUG

	if (isDead_)
	{
		SceneManager::GetInstance()->ChangeScene("TitleScene");
	}
}

void Player::Draw()
{
	// 当たっていたら非表示にする
	if (isHit_)
	{
		return;
	}

	object3d_->Draw();
	lockOn_->Draw();

#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG
}

void Player::Finalize()
{}

void Player::OnCollision()
{
	//isDead_ = true;
}

void Player::CreateBullet()
{
	
	if (lockOn_->GetTarget())
	{
		targetPosition = lockOn_->GetTarget()->GetWorldTranslate();
	}
	else
	{
		targetPosition = reticleTransform_.translate;
	}

	velocity = Vector3Subtract(targetPosition,transform_.translate);

	velocity = Normalize(velocity);

	isRailCamera = true;

	// 速度を算出
	velocity = FloatMultiply(velocity, kBulletSpeed_);

	// 生成と初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize(transform_, filePath_, velocity,isRailCamera);
	BulletManager::GetInstance()->SetBullets(std::move(bullet));

}

void Player::UpdateReticle()
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
	Matrix4x4 reticleWorldMatrix = MakeAffineMatrixQuat(reticleTransform_.scale, reticleTransform_.rotate, reticleTransform_.translate);
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
