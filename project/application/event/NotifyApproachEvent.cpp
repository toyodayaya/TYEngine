#include "NotifyApproachEvent.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void NotifyApproachEvent::Initialize(const QuaternionTransform& transform)
{
	// ワールドトランスフォームを初期化
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxRotate(transform.rotate);
	debugDraw->SetBoxTranslate(transform.translate);
#endif // _DEBUG
}

void NotifyApproachEvent::Finalize()
{
#ifdef _DEBUG
	debugDraw.reset();
#endif // _DEBUG
}

void NotifyApproachEvent::Update()
{
#ifdef _DEBUG
	// デバッグ描画の更新処理
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void NotifyApproachEvent::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void NotifyApproachEvent::OnCollision(const std::string& hitObjectType)
{
	// 接触していたら処理しない
	if (isHit_)
	{
		return;
	}

	// 接触したのがプレイヤーだったら
	if (hitObjectType == "PlayerSpawn")
	{
		// 登録された敵に接近を通知
		for (auto& enemy : enemies_)
		{
			enemy->SetIsApproach(true);
		}

		// 接触フラグを立てる
		isHit_ = true;
	}

}

void NotifyApproachEvent::RegisterEnemy()
{
	// ステージ内の敵とイベントの当たり判定を走査
	for (auto& enemy : EnemyManager::GetInstance()->GetEnemies())
	{
		// 敵のAABBを取得
		AABB enemyAABB = enemy->GetAABB();

		// 判定
		if (CollisionManager::GetInstance()->IsCollision(aabb_, enemyAABB))
		{
			// 接触している敵をリスト登録
			enemies_.push_back(enemy.get());
		}
	}
}
