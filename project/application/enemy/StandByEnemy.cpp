#include "StandByEnemy.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void StandByEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = enemy;

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };

	// オブジェクトの不透明度をセット
	object3d_->SetAlpha(0.0f);

	

}

void StandByEnemy::Update()
{
	// 接近フラグが立ったら
	if (enemy_->GetIsApproach())
	{
		if (!isEmit_)
		{
			// エフェクトを発生させる
			EmitParticle();
		}
		// 不透明度を変更
		AddAlphaValue();
	}

	// オブジェクトを更新
	object3d_->Update();
}

void StandByEnemy::Draw()
{
	// オブジェクトを描画する
	object3d_->Draw();
}

void StandByEnemy::Finalize()
{}

void StandByEnemy::OnCollision(const std::string& hitObjectType)
{

}

void StandByEnemy::AddAlphaValue()
{
	// タイマーを加算
	alphaTimer_ += kDeltaTime;

	// タイマーが既定値を超えたら
	if (alphaTimer_ >= 0.5f)
	{
		// 指定された状態に移行
		enemy_->ChangeEnemyState(enemy_->GetNextStateName());
		object3d_->SetAlpha(1.0f);
	}
	else
	{
		// アルファ値を加算
		object3d_->SetAlpha(alphaTimer_);
	}
}

void StandByEnemy::EmitParticle()
{
	// パーティクル初期値を初期化
	emitterSphere.translate = object3d_->GetWorldTranslate();
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kNone;
	emitterSphere.scale = Vector3{ 2.0f,2.0f,2.0f };
	emitterSphere.lifeTime = 1.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.0f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 1;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	emitterSphere.radius = 1.0f;
	particleId_ = ParticleManager::GetInstance()->AllocateID();
	emitterName = "StandByEnemy_" + std::to_string(particleId_);
	ParticleManager::GetInstance()->CreateParticleGroup(emitterName, "resources/sprite/particle/appear.png", ParticleManager::ShapeType::kNormal);
	emitter = std::make_unique <ParticleEmitter>(emitterName, emitterSphere);
	emitter->Update();
	isEmit_ = true;
}

