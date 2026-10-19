#include "NormalEnemy.h"
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

void NormalEnemy::OnCollision()
{
	if (isHit_)
	{
		// 既に当たっていたら処理しない
		return;
	}


	// HPを減らす
	hp_--;

	// HPが0になったら
	if (hp_ <= 0)
	{
		// ステートを変更する
		//enemy_->ChangeEnemyState("DeadEnemy");
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
