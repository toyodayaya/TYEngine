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

void StandByEnemy::OnCollision(std::string hitObjectType)
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
	}
	else
	{
		// アルファ値を加算
		object3d_->SetAlpha(alphaTimer_);
	}
}

