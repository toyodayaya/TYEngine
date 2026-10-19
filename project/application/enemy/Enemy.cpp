#include "Enemy.h"
#include "Object3dCommon.h"
#include "DamageManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG


void Enemy::Initialize(const QuaternionTransform& transform, const std::string& filePath)
{
	// 3Dオブジェクトを初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel(filePath);
	object3d_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	object3d_->SetTransform(transform);
	transform_ = transform;
	isDead_ = false;
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);

#endif // _DEBUG
}

void Enemy::Finalize()
{

}

void Enemy::Update()
{

	// 敵ステート切り替え

	// 次敵ステートの予約があったら
	if (nextEnemyState_)
	{
		// ステートを変更する
		ChangeEnemyState(std::move(nextEnemyState_));
	}

	// 敵を更新する
	if (isHit_)
	{
		hitTimer_--;

		if (hitTimer_ <= 0)
		{
			isHit_ = false;
		}

	}

	// ステートを更新する
	state_->Update();

#ifdef _DEBUG
	debugDraw->UpdateBox();
#endif // _DEBUG

}

void Enemy::Draw()
{
	// ステートを描画する
	state_->Draw();
#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG
}

void Enemy::OnCollision(std::string hitObjectType)
{
	// 衝突応答
	state_->OnCollision(hitObjectType);
}
