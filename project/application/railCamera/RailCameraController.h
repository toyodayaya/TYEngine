#pragma once
#include <vector>
#include "Camera.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

class RailCameraController
{
public:
	// 初期化
	void Initialize(const QuaternionTransform& transform);
	// 更新
	void Update();
	// 描画
	void Draw();

	// カメラを動かす処理
	void AutoMove();
	void ManualMove();

	// setter
	void SetCamera(Camera* camera) { this->camera_ = camera; }
	void SetRailPoint(const std::vector<Vector3>& railPoint) { railPoints_ = railPoint; }

private:
	// ワールド座標
	QuaternionTransform transform_;
	// カメラ
	Camera* camera_ = nullptr;

	// カメラの制御点
	std::vector<Vector3> railPoints_;
	// 前の制御点
	Vector3 prePoint_;
	// 今の制御点
	Vector3 nowPoint_;
	// 次の制御点
	Vector3 nextPoint_;
	// 制御点の数
	int currentRailPointNum_ = 0;
	// 移動フラグ
	bool isMove_ = false;

	// 線形補間用の変数
	float t = 0.0f;
	const float offset_ = 0.001f;

#ifdef _DEBUG
	// デバッグ描画
	std::vector<std::unique_ptr<DebugDraw>> debugPoint_;
	std::unique_ptr<DebugDraw> debugLine_;
#endif // _DEBUG
};

