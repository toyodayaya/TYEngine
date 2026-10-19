#pragma once
#include "Object3d.h"
#include "GameObject.h"
#include "BaseEnemyState.h"
#include "EnemyStateFactory.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"
#endif // _DEBUG

class BaseEnemy : public GameObject
{
public:
	// 初期化
	virtual void Initialize(const QuaternionTransform& transform, const std::string& filePath) = 0;
	// 次状態予約
	void ChangeEnemyState(const std::string& enemyState);
	// 敵ステートを変更する
	void ChangeEnemyState(std::unique_ptr<BaseEnemyState> nextState);
	// 敵ステートファクトリーのセット
	void SetEnemyStateFactory(std::unique_ptr<AbstractEnemyStateFactory> enemyStateFactory) { enemyStateFactory_ = std::move(enemyStateFactory); }
	// getter
	Object3d* GetObject3d() { return object3d_.get(); }

protected:
	// 当たり判定用のAABB
	AABB aabb_;
	// HP
	int hp_ = 10;
	// ヒットタイマー
	int hitTimer_ = 3;
	// ヒットフラグ
	bool isHit_ = false;

	// 3dオブジェクト
	std::unique_ptr<Object3d> object3d_;

	// 次敵ステート
	std::unique_ptr<BaseEnemyState> nextEnemyState_ = nullptr;
	// 敵ステートファクトリー
	std::unique_ptr<AbstractEnemyStateFactory> enemyStateFactory_ = nullptr;

	// 敵ステート名を記録
	std::string enemyStateName_;

	// 敵の状態
	std::unique_ptr<BaseEnemyState> state_;


#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG
};

