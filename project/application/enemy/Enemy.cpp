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
	object3d_->Initialize(Object3dCommon::GetInstance());
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
	if (isHit_)
	{
		hitTimer_--;

		if (hitTimer_ <= 0)
		{
			isHit_ = false;
		}

	}

	object3d_->Update();

#ifdef _DEBUG
	debugDraw->UpdateBox();
#endif // _DEBUG

}

void Enemy::Draw()
{
	object3d_->Draw();

#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG
}

void Enemy::OnCollision()
{
	if (!isHit_ && !isDead_)
	{
		hp_ -= 5;

		if (hp_ <= 0)
		{
			isDead_ = true;
		}
		else
		{
			isHit_ = true;
			DamageManager::GetInstance()->AddScore(10);
		}
	}
}
