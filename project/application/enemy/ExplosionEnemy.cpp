#include "ExplosionEnemy.h"
#include "Enemy.h"
#include "Explosion.h"
#include "PlayerManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"

#endif // _DEBUG

void ExplosionEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = dynamic_cast<Enemy*>(enemy);

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };

	// 爆発準備
	CreateExplosion();
}

void ExplosionEnemy::Update()
{
	// オブジェクトを更新
	object3d_->Update();
}

void ExplosionEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void ExplosionEnemy::Finalize()
{}

void ExplosionEnemy::OnCollision(const std::string& hitObjectType)
{
	if (enemy_->IsHit())
	{
		// 既に当たっていたら処理しない
		return;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "PlayerSpawn")
	{

	}
	else if (hitObjectType == "PlayerBulletSpawn")
	{
		enemy_->Damage();
	}
}

void ExplosionEnemy::CreateExplosion()
{
	// 爆発を用意
	explosion = std::make_unique<Explosion>();
	explosion->Initialize(enemy_, transform_, Vector3{ 0.0f,0.0f,0.0f }, false, "EnemyBulletSpawn");
	explosion->SetID(ParticleManager::GetInstance()->AllocateID());
	explosion->SetEnemy(this);

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(explosion));
}
