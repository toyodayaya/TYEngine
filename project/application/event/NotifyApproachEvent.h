#pragma once
#include "BaseEvent.h"
#include "EventManager.h"
#include "BaseEnemy.h"
#include <vector>
#ifdef _DEBUG
#include "DebugDraw.h"
#endif // _DEBUG

class NotifyApproachEvent : public BaseEvent
{
public:
	// 初期化
	void Initialize(const QuaternionTransform& transform) override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 衝突応答
	void OnCollision(const std::string& hitObjectType) override;

	// 範囲内にいる敵を登録する処理
	void RegisterEnemy();

private:

	// 登録済みの敵を記録する配列
	std::vector<BaseEnemy*> enemies_;

#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG
};

