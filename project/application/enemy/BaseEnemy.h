#pragma once
#include "Object3d.h"
#include "BaseCharacter.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"
#endif // _DEBUG

class BaseEnemy : public BaseCharacter
{
public:
	// 初期化
	virtual void Initialize(const QuaternionTransform& transform, const std::string& filePath) = 0;
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


#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG
};

