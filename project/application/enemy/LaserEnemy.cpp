#include "LaserEnemy.h"
#include "Enemy.h"
#include "Explosion.h"
#include "PlayerManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"

#endif // _DEBUG

void LaserEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = dynamic_cast<Enemy*>(enemy);

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };

	// パーティクル初期値を初期化
	emitterSphere.translate = transform_.translate;
	emitterSphere.translate.x -= (laserSize.x / 2.0f);
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kNone;
	emitterSphere.scale = laserSize;
	emitterSphere.lifeTime = 10.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.5f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 1;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	emitterSphere.radius = 0.0f;
	particleId_ = ParticleManager::GetInstance()->AllocateID();
	emitterName = "EnemyLaser_" + std::to_string(particleId_);
	ParticleManager::GetInstance()->CreateParticleGroup(emitterName, "resources/sprite/particle/laser.png", ParticleManager::ShapeType::kNormal);
	emitter = std::make_unique <ParticleEmitter>(emitterName, emitterSphere);

	// エフェクトを発生させる
	emitter->Update();

	// レーザー準備
	CreateLaser();
}

void LaserEnemy::Update()
{
	// オブジェクトを更新
	object3d_->Update();
}

void LaserEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void LaserEnemy::Finalize()
{}

void LaserEnemy::OnCollision(const std::string& hitObjectType)
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

void LaserEnemy::CreateLaser()
{
	// レーザーを用意
	laser = std::make_unique<Laser>();
	laser->Initialize(transform_, Vector3{ 0.0f,0.0f,0.0f }, false, "EnemyBulletSpawn");
	laser->SetID(ParticleManager::GetInstance()->AllocateID());

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(laser));
}
