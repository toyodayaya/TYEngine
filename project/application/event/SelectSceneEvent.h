#pragma once
#include "BaseEvent.h"
#include "EventManager.h"
#include <string>
#include <memory>
#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
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
	void SetStageFilePath(std::string filePath) { objectFilePath_ = filePath; }

private:
	
	// シーン遷移フラグ
	bool isChangeScene_ = false;

	// ステージ名
	std::string stageName_;

	// ステージ名のスプライト
	std::unique_ptr<Sprite> stageNameSprite_;
	// ステージのミニオブジェクト
	std::unique_ptr<Object3d> object3d_;
	// ミニオブジェクトのファイルパス
	std::string objectFilePath_;

#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};

