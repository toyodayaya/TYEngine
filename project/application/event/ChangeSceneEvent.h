#pragma once
#include "BaseEvent.h"
#include "EventManager.h"
#include "Object3d.h"
#include <string>
#include <memory>

#ifdef _DEBUG
#include "DebugDraw.h"
#endif // _DEBUG

class ChangeSceneEvent : public BaseEvent
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
	void SetObjectFilePath(std::string filePath) { objectFilePath_ = filePath; }


private:
	// ステージのミニオブジェクト
	std::unique_ptr<Object3d> object3d_;
	// ミニオブジェクトのファイルパス
	std::string objectFilePath_;


#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};

