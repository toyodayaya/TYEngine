#pragma once
#include "BasePlayerState.h"
#include "PlayerStateFactory.h"
#include <memory>
#include "Camera.h"
#include "Object3d.h"
#include "Player.h"
#include "MathManager.h"
using namespace MathManager;

class PlayerManager
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class PlayerManager;
	};

	// PassKeyを受け取るコンストラクタ
	explicit PlayerManager(ConstructorKey) {}

private:

	// デストラクタ
	~PlayerManager() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	PlayerManager(const PlayerManager&) = delete;
	PlayerManager& operator=(const PlayerManager&) = delete;
	// インスタンス
	friend std::default_delete<PlayerManager>;
	static std::unique_ptr<PlayerManager> instance;

public:
	// 初期化
	void Initialize(const QuaternionTransform& transform, const std::string filePath, Camera* camera);
	// 次状態予約
	void ChangePlayerState(const std::string& playerState);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();
	
	// インスタンス
	static PlayerManager* GetInstance();

	// プレイヤーステートファクトリーのセット
	void SetPlayerStateFactory(std::unique_ptr<AbstractPlayerStateFactory> playerStateFactory) { playerStateFactory_ = std::move(playerStateFactory); }

	// getter
	const std::unique_ptr<Player>& GetPlayer()  { return player_; }
	const std::string& GetPlayerState() { return playerStateName_; }

private:
	// プレイヤーの本体ポインタ
	std::unique_ptr<Player> player_;
	// 次プレイヤーステート
	std::unique_ptr<BasePlayerState> nextPlayerState_ = nullptr;
	// プレイヤーステートファクトリー
	std::unique_ptr<AbstractPlayerStateFactory> playerStateFactory_ = nullptr;

	// プレイヤーステート名を記録
	std::string playerStateName_;
};

