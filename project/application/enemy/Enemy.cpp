#include "Enemy.h"
#include "Object3dCommon.h"
#include "DamageManager.h"

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
}

void Enemy::Draw()
{
	object3d_->Draw();
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
