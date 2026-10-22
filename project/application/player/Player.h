#pragma once
#include "Object3d.h"
#include <memory>
#include "GameObject.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Camera.h"
#include "BasePlayerState.h"
#include "InGameUI.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"
#endif // _DEBUG


class Player : public GameObject
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
	void OnCollision(std::string hitObjectType) override;

	// プレイヤーステートを変更する
	void ChangePlayerState(std::unique_ptr<BasePlayerState> nextState);

	// setter
	void SetCamera(Camera* camera) { this->camera_ = camera; }
	void SetWorldMatrix(const Matrix4x4& matrix) { worldMatrix = matrix; }
	void SetHP(int hp) { hp_ = hp; }

	// getter
	Object3d* GetObject3d() { return object3d_.get(); }
	Object3d* GetReticle() { return reticle_.get(); }
	Camera* GetCamera() { return camera_; }
	BasePlayerState* GetPlayerState() { return state_.get(); }
	InGameUI* GetInGameUI() { return inGameUI_.get(); }
	LockOn* GetLockOn() { return lockOn_.get(); }
	const int GetMaxHP() const { return kMaxHp; }
	int GetHP() { return hp_; }

private:

	// ファイル名
	std::string filePath_ = "cube.obj";

	// HP
	const int kMaxHp = 2;
	int hp_ = 2;

	// カメラ
	Camera* camera_ = nullptr;

	// 3dオブジェクト
	std::unique_ptr<Object3d> object3d_;

	// プレイヤーの状態
	std::unique_ptr<BasePlayerState> state_;

	// UI
	std::unique_ptr<InGameUI> inGameUI_;

	// 3Dレティクルオブジェクト
	std::unique_ptr<Object3d> reticle_;
	// 3Dレティクルのワールドトランスフォーム
	QuaternionTransform reticleTransform_;
	// 自機から3Dレティクルへのオフセット
	Vector3 reticleOffset_ = { 0.0f,0.0f,10.0f };

	// ロックオンのポインタ
	std::unique_ptr<LockOn> lockOn_;
	
#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};
