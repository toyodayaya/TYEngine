#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include <memory>
#include "GameObject.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Camera.h"
#include "BasePlayerState.h"

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

	// getter
	Object3d* GetObject3d() { return object3d_.get(); }
	Camera* GetCamera() { return camera_; }
	BasePlayerState* GetPlayerState() { return state_.get(); }
	const std::unique_ptr<Sprite>& GetMoveSprite() const { return moveSprite_; }
	const std::vector<std::unique_ptr<Sprite>>& GetHpSprite() const { return hpSprite_; }
	const std::vector<std::unique_ptr<Sprite>>& GetBeamSprite() const { return beamSprite_; }


private:

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

	// UI
	std::unique_ptr<Sprite> moveSprite_;
	std::vector<std::unique_ptr<Sprite>> hpSprite_;
	std::vector<std::unique_ptr<Sprite>> beamSprite_;

#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};
