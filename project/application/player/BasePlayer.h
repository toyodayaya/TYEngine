#pragma once
#include "BaseCharacter.h"
#include "Camera.h"

class BasePlayer : public BaseCharacter
{
public:
	// 初期化
	virtual void Initialize(const QuaternionTransform& transform, const std::string& filePath, bool isRailcamera, Camera* camera) = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// 終了
	virtual void Finalize() = 0;

	// 仮想デストラクタ
	virtual ~BasePlayer() = default;

private:
	
};

