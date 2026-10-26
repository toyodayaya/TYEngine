#pragma once
#include "Object3d.h"
#include <memory>
#include "BasePlayerState.h"
#include "Camera.h"
#include "Player.h"

class SelectScenePlayer : public BasePlayerState
{
public:
	// 初期化
	void Initialize(Player* player) override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Finalize() override;

	// 衝突応答
	void OnCollision(std::string hitObjectType) override;
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }

private:
	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	// カメラから自機へのオフセット
	Vector3 playerOffset_ = { 0.0f,0.0f,10.0f };

	// カメラ
	Camera* camera_ = nullptr;

	// 3dオブジェクト
	Object3d* object3d_;
};