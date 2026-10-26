#pragma once
#include "BaseEvent.h"
#include "EventManager.h"
#include <string>

#ifdef _DEBUG
#include "DebugDraw.h"
#endif // _DEBUG

class SelectSceneEvent : public BaseEvent
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
	void OnCollision(std::string hitObjectType) override;

	// setter
	void SetStageName(std::string stageName) { stageName_ = stageName; }

private:

	// シーン遷移フラグ
	bool isChangeScene_ = false;

	// ステージ名
	std::string stageName_;

#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};

