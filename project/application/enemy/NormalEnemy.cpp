#include "NormalEnemy.h"
#include "PlayerManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void NormalEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = enemy;

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };
	isHit_ = false;
}

void NormalEnemy::Update()
{
	// ヒットフラグが立っていたら
	if (isHit_)
	{
		// ヒットタイマーを更新する
		HitTimerUpdate();
	}

	// オブジェクトを更新
	object3d_->Update();
}

void NormalEnemy::Draw()
{
	// ヒットフラグが立っていたら
	if (isHit_)
	{
		// 点滅させる
		if (hitTimer_ % 2 == 0.0f)
		{
			object3d_->Draw();
		}
	}
	else
	{
		// オブジェクトを描画する
		object3d_->Draw();
	}

}

void NormalEnemy::Finalize()
{}

void NormalEnemy::OnCollision(std::string hitObjectType)
{
	if (isHit_)
	{
		// 既に当たっていたら処理しない
		return;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "PlayerSpawn")
	{

	}
	else if (hitObjectType == "BulletSpawn")
	{
		// HPを減らす
		hp_--;

		// ビームポイントを加算
		PlayerManager::GetInstance()->SetBeamPoint(5);

		// HPが0になったら
		if (hp_ <= 0)
		{
			// パーティクル初期値を初期化
			emitterSphere.velocity = { 0.0f,0.0f,0.0f };
			emitterSphere.type = ParticleManager::MoveType::kDiffusion;
			emitterSphere.scale = { 3.0f,3.0f,3.0f };
			emitterSphere.lifeTime = 3.0f;
			emitterSphere.frequency = 0.0f;
			emitterSphere.frequencyTime = 0.0f;
			emitterSphere.currentTime = 0;
			emitterSphere.count = 8;
			emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
			emitterSphere.translate = object3d_->GetWorldTranslate();
			emitterSphere.id = 0;
			emitter = std::make_unique <ParticleEmitter>("DeadEnemy", emitterSphere);

			// パーティクル発射
			emitter->Update();

			// ビームポイントパーティクルに発生を通知
			PlayerManager::GetInstance()->SetParticlePoint(object3d_->GetWorldTranslate());

			// 本体に死亡を通知
			enemy_->SetIsDead(true);
		}
	}

	// ヒットフラグを立てる
	isHit_ = true;

}

void NormalEnemy::HitTimerUpdate()
{
	// ヒットタイマーが0になったら
	if (hitTimer_ <= 0)
	{
		// フラグを元に戻す
		isHit_ = false;
		hitTimer_ = 60;
	}
	else
	{
		// ヒットタイマーを減らす
		hitTimer_--;
	}
}
