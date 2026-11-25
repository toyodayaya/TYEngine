#pragma once
#include "BaseBullet.h"
#include <string>

class Laser : public BaseBullet
{
public:
	// 初期化
	void Initialize(GameObject* parent, const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType) override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 衝突応答
	void OnCollision(const std::string& hitObjectType) override;

private:
	bool isHit_ = false;

};

