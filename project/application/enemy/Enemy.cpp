#include "Enemy.h"
#include "PlayerManager.h"
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
	debugDraw->Initialize("resources/human/white.png", DebugDraw::DrawState::kBox);
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
		HitTimerUpdate();
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

void Enemy::OnCollision(const std::string& hitObjectType)
{
	// 衝突応答
	state_->OnCollision(hitObjectType);
}

void Enemy::Damage()
{
	// HPを減らす
	hp_--;

	// ヒットフラグを立てる
	isHit_ = true;

	// HPが0になったら
	if (hp_ <= 0)
	{
		// ビームポイントを加算
		PlayerManager::GetInstance()->SetBeamPoint(5);

		// パーティクル初期値を初期化
		emitterSphere.velocity = { 0.0f,0.0f,0.0f };
		emitterSphere.type = ParticleManager::MoveType::kNone;
		emitterSphere.scale = { 2.0f,2.0f,2.0f };
		emitterSphere.lifeTime = 2.0f;
		emitterSphere.frequency = 0.0f;
		emitterSphere.frequencyTime = 0.0f;
		emitterSphere.currentTime = 0;
		emitterSphere.count = 1;
		emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
		emitterSphere.translate = object3d_->GetWorldTranslate();
		emitterSphere.id = 0;
		emitter = std::make_unique <ParticleEmitter>("DeadEnemy", emitterSphere);

		// パーティクル発射
		emitter->Update();

		// ビームポイントパーティクルに発生を通知
		PlayerManager::GetInstance()->SetParticlePoint(object3d_->GetWorldTranslate());

		// 本体に死亡を通知
		isDead_ = true;
	}
}

void Enemy::HitTimerUpdate()
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

void Enemy::DamageEffectDraw()
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

void Enemy::Rotate()
{
	// プレイヤーの方に向く
	Vector3 target = PlayerManager::GetInstance()->GetPlayerTranslate();
	Vector3 forward = Vector3Subtract(target, transform_.translate);
	forward = FloatMultiply(Normalize(forward), -1.0f);
	// Y軸周り角度
	transform_.rotate.y = std::atan2(forward.x, forward.z);
	float yaw = transform_.rotate.y * -1.0f;
	// Y軸周りの回転行列
	Matrix4x4 rotate = MakeRotateYMatrix(yaw);
	Vector3 velocityZ = TransformNormal(forward, rotate);
	// X軸周り角度
	transform_.rotate.x = std::atan2(-velocityZ.y, velocityZ.z);

	// オブジェクトに反映
	object3d_->SetTransform(transform_);
}
