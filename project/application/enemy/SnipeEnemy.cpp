#include "SnipeEnemy.h"
#include "Enemy.h"
#include "PlayerManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"

#endif // _DEBUG

void SnipeEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = dynamic_cast<Enemy*>(enemy);

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };

	// パーティクル初期値を初期化
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kFollow;
	emitterSphere.scale = { 1.0f,1.0f,1.0f };
	emitterSphere.lifeTime = 3.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.0f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 1;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	particleId_ = ParticleManager::GetInstance()->AllocateID();
	emitterName = "EnemyBullet_" + std::to_string(particleId_);
	ParticleManager::GetInstance()->CreateParticleGroup(emitterName, "resources/sprite/circle.png", ParticleManager::ShapeType::kNormal);
	emitter = std::make_unique <ParticleEmitter>(emitterName, emitterSphere);
}

void SnipeEnemy::Update()
{
	// タイマーを更新
	waitTime_ -= kDeltaTime;

	// タイマーが0になったら
	if (waitTime_ <= 0.0f)
	{
		// 弾を発射
		CreateBullet();
	}

	// プレイヤーの方を向く処理
	enemy_->Rotate();

	// オブジェクトを更新
	object3d_->Update();
}

void SnipeEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void SnipeEnemy::Finalize()
{}

void SnipeEnemy::OnCollision(std::string hitObjectType)
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

void SnipeEnemy::CreateBullet()
{
	// 速度を算出
	Vector3 target = PlayerManager::GetInstance()->GetPlayerTranslate();
	velocity_ = Vector3Subtract(target, transform_.translate);
	velocity_ = Normalize(velocity_);
	velocity_ = FloatMultiply(velocity_, kBulletSpeed_);

	// 弾を発射
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize(transform_, velocity_, false, "EnemyBulletSpawn");
	bullet->SetID(ParticleManager::GetInstance()->AllocateID());

	// エフェクトを追加
	emitterSphere.translate = transform_.translate;
	emitterSphere.scale = { 1.0f,1.0f,1.0f };
	emitterSphere.id = bullet->GetID();
	emitter->SetEmitterSphere(emitterSphere);
	// エフェクトを発生させる
	emitter->Update();
	// 対応するエフェクトに生存フラグを通知
	ParticleManager::GetInstance()->SetBulletIsAlive(true, bullet->GetID(), emitterName);
	// パーティクルグループの名前を記録
	bullet->SetParticleName(emitterName);

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(bullet));

	// 発射間隔タイマーをリセット
	waitTime_ = 0.5f;
}
