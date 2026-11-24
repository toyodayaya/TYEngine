#include "GasEnemy.h"
#include "Enemy.h"
#include "Explosion.h"
#include "PlayerManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"

#endif // _DEBUG

void GasEnemy::Initialize(BaseEnemy* enemy)
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
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kDiffusion;
	emitterSphere.scale = { 1.0f,1.0f,1.0f };
	emitterSphere.lifeTime = 3.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.5f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 16;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	emitterSphere.radius = 30.0f;
	particleId_ = ParticleManager::GetInstance()->AllocateID();
	emitterName = "EnemyGas_" + std::to_string(particleId_);
	ParticleManager::GetInstance()->CreateParticleGroup(emitterName, "resources/sprite/particle/gas.png", ParticleManager::ShapeType::kNormal);
	emitter = std::make_unique <ParticleEmitter>(emitterName, emitterSphere);

	// ガス準備
	CreateGas();
}

void GasEnemy::Update()
{
	// エフェクトを更新
	emitter->Update();

	// オブジェクトを更新
	object3d_->Update();
}

void GasEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void GasEnemy::Finalize()
{}

void GasEnemy::OnCollision(std::string hitObjectType)
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

void GasEnemy::CreateGas()
{
	// 爆発を用意
	gas = std::make_unique<Gas>();
	gas->Initialize(transform_, Vector3{ 0.0f,0.0f,0.0f }, false, "EnemyBulletSpawn");
	gas->SetID(ParticleManager::GetInstance()->AllocateID());

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(gas));
}
