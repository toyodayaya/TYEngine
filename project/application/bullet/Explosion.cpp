#include "Explosion.h"
#include "CollisionManager.h"
#include "StageData.h"
#include "Object3dCommon.h"
#include "ImGuiManager.h"
#include "ScoreManager.h"
#include "ParticleManager.h"
#include "PlayerManager.h"
#include "ExplosionEnemy.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void Explosion::Initialize(const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType)
{
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(FloatMultiply(transform.scale, 5.0f));
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);

#endif // _DEBUG

	// コライダーを生成
	StageData::ColliderSpawnData colliders;
	colliders.center = Vector3{ 0.0f,0.0f,0.0f };
	colliders.size = FloatMultiply(transform.scale, 5.0f);
	colliders.parent = this;
	colliders.objectType = objectType;
	objectName_ = objectType;
	CollisionManager::GetInstance()->SetColliders(colliders);

	isDead_ = false;

	// パーティクル初期値を初期化
	emitterSphere.translate = transform_.translate;
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kDiffusion;
	emitterSphere.scale = { 1.0f,1.0f,1.0f };
	emitterSphere.lifeTime = 2.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.0f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 8;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	emitterSphere.radius = 10.0f;
	id_ = ParticleManager::GetInstance()->AllocateID();
	std::string emitterName = "Explosion_" + std::to_string(id_);
	ParticleManager::GetInstance()->CreateParticleGroup(emitterName, "resources/sprite/particle/explosion.png", ParticleManager::ShapeType::kNormal);
	emitter = std::make_unique <ParticleEmitter>(emitterName, emitterSphere);
}

void Explosion::Finalize()
{

}

void Explosion::Update()
{
	if (isWait_)
	{
		// 爆発までの時間を更新
		waitTime_ += kDeltaTime;

		if (waitTime_ >= 0.1f)
		{
			// 親オブジェクトを透明にする
			enemy_->GetObject3d()->SetColor(Vector4{ 0.0f,0.0f,0.0f,0.0f });
			// 時間を過ぎたら待機状態から移行
			isWait_ = false;
			isExplosion_ = true;
		}
	}
	else if (isExplosion_)
	{
		emitter->Update();

		// 爆発までの時間を更新
		explosionTime_ += kDeltaTime;

		if (explosionTime_ >= 0.3f)
		{
			// 時間を過ぎたら死亡フラグをON
			isDead_ = true;
			// 親オブジェクトに死亡を通知する
			enemy_->SetIsDead(true);
		}
	}

	// 弾のローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	debugDraw->SetBoxTranslate(transform_.translate);
	debugDraw->UpdateBox();
#endif // _DEBUG

#ifdef USE_IMGUI
	ImGui::Begin("bullet");
	ImGui::DragFloat3("pos", &transform_.translate.x);

	ImGui::End();

#endif // USE_IMGUI

}

void Explosion::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG


}

void Explosion::OnCollision(std::string hitObjectType)
{
	if (isHit_)
	{
		return;
	}

	if (objectName_ == "PlayerBulletSpawn")
	{
		// 当たったオブジェクトのタイプで分岐
		if (hitObjectType == "EnemySpawn")
		{
			// 敵だったらスコアを加算
			ScoreManager::GetInstance()->AddScore(10);
			ScoreManager::GetInstance()->SetNumber();
			// 弾の死亡フラグを立てる
			isDead_ = true;
			// 対応するエフェクトに死亡を通知
			ParticleManager::GetInstance()->SetBulletIsAlive(false, id_, particleName_);

		}
	}
	else if (objectName_ == "EnemyBulletSpawn")
	{
		// 当たったオブジェクトのタイプで分岐
		if (hitObjectType == "PlayerSpawn")
		{
			if (isExplosion_)
			{

				// プレイヤーのHPを減らす
				PlayerManager::GetInstance()->SetPlayerHP(1);
				PlayerManager::GetInstance()->SetIsHit(true);
				isHit_ = true;

			}
			else if (!isWait_)
			{
				// 爆発待機状態に移行
				isWait_ = true;
				// 親オブジェクトを赤色にする
				enemy_->GetObject3d()->SetColor(Vector4{ 1.0f,0.0f,0.0f,1.0f });
			}

		}
	}


}
