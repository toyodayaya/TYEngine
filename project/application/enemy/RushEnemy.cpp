#include "RushEnemy.h"
#include "Enemy.h"
#include "PlayerManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"

#endif // _DEBUG

void RushEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = dynamic_cast<Enemy*>(enemy);

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };
}

void RushEnemy::Update()
{
	// 状態によって分岐
	switch(phase_)
	{
	case kWait:

		// 待機状態の更新処理
		Wait();

		break;

	case kRush:
		
		// 突進時の更新処理
		Rush();

		break;
	}

	// オブジェクトを更新
	object3d_->Update();
}

void RushEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void RushEnemy::Finalize()
{}

void RushEnemy::OnCollision(std::string hitObjectType)
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

void RushEnemy::Wait()
{
	// タイマーを更新
	waitTime_ -= kDeltaTime;

	// タイマーが0になったら
	if (waitTime_ <= 0.0f)
	{
		// 速度を算出
		Vector3 target = PlayerManager::GetInstance()->GetPlayerTranslate();
		velocity_ = Vector3Subtract(target, transform_.translate);
		velocity_ = Normalize(velocity_);
		velocity_ = FloatMultiply(velocity_, kRushSpeed_);

		// 状態を変更
		phase_ = kRush;
	}
	else
	{
		// プレイヤーの方を向く処理
		enemy_->Rotate();
	}
}

void RushEnemy::Rush()
{
	// 座標を更新
	transform_.translate = Vector3Add(transform_.translate, velocity_);
	object3d_->SetTranslate(transform_.translate);
	// オブジェクトのローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	// オブジェクトのワールド座標をセット
	object3d_->SetWorldMatrix(worldMatrix);
}