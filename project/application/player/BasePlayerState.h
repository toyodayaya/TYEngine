#pragma once
#include <memory>

class Player;

class BasePlayerState
{
public:
	// 初期化
	virtual void Initialize(Player* player) = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// 終了
	virtual void Finalize() = 0;
	// 当たり判定
	virtual void OnCollision() = 0;

	// 仮想デストラクタ
	virtual ~BasePlayerState() = default;

private:
	
	
};

