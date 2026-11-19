#include "NormalEnemy.h"
#include "Enemy.h"
#include "PlayerManager.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void NormalEnemy::Initialize(BaseEnemy* enemy)
{
	// 引数で受け取ってメンバ変数に記録
	enemy_ = dynamic_cast<Enemy*>(enemy);

	// オブジェクトの初期化
	object3d_ = enemy_->GetObject3d();
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };

	// オブジェクトの不透明度をセット
	object3d_->SetAlpha(0.0f);
}

void NormalEnemy::Update()
{
	// 接近フラグが立ったら
	if (enemy_->GetIsApproach())
	{
		// 不透明度を変更
		object3d_->SetAlpha(1.0f);
	}

	// オブジェクトを更新
	object3d_->Update();
}

void NormalEnemy::Draw()
{
	enemy_->DamageEffectDraw();
}

void NormalEnemy::Finalize()
{}

void NormalEnemy::OnCollision(std::string hitObjectType)
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
	else if (hitObjectType == "BulletSpawn")
	{
		enemy_->Damage();
	}

}

