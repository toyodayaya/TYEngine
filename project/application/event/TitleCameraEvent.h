#pragma once
#include "BaseEvent.h"
#include "EventManager.h"
#include "Camera.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#endif // _DEBUG

class TitleCameraEvent : public BaseEvent
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
	void OnCollision() override;

	// カメラを動かす関数
	void Move();

	// setter
	void SetCamera(Camera* camera) { this->camera_ = camera; }

private:
	// カメラ
	Camera* camera_ = nullptr;
	
	// カメラ移動フラグ
	bool isMoveCamera_ = false;
	// 線形補間用の変数
	float t_ = 0.0f;

	// カメラの角度
	Quaternion angle_;
	Quaternion targetAngle_;
	Quaternion startAngle_;
};

