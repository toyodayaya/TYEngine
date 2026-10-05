#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include <memory>
#include "BaseCharacter.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Camera.h"
#include "BasePlayerState.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"
#endif // _DEBUG


class Player : public BaseCharacter
{
public:
	// 初期化
	void Initialize(const QuaternionTransform& transform, const std::string& filePath, Camera* camera);
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Finalize() override;

	// 衝突応答
	void OnCollision() override;

	// プレイヤーステートを変更する
	void ChangePlayerState(std::unique_ptr<BasePlayerState> nextState);

	// setter
	void SetCamera(Camera* camera) { this->camera_ = camera; }
	void SetWorldMatrix(const Matrix4x4& matrix) { worldMatrix = matrix; }

	// getter
	Object3d* GetObject3d() { return object3d_.get(); }
	Camera* GetCamera() { return camera_; }

private:

	// 当たり判定フラグ
	bool isHit_;
	// ファイル名
	std::string filePath_ = "cube.obj";

	// HP
	int hp_ = 2;

	// カメラ
	Camera* camera_ = nullptr;

	// 3dオブジェクト
	std::unique_ptr<Object3d> object3d_;

	// プレイヤーの状態
	std::unique_ptr<BasePlayerState> state_;

#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};
