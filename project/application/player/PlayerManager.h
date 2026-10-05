#pragma once
#include "BasePlayer.h"
#include "PlayerStateFactory.h"
#include <memory>
#include "Camera.h"
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

	// setter
	void SetPlayerData(const QuaternionTransform& transform, const std::string& filePath, bool isRailcamera,Camera* camera);
	// getter
	const std::unique_ptr<BasePlayer>& GetNextPlayer()  { return nextPlayer_; }

private:
	// 実行中のプレイヤーステート
	std::unique_ptr<BasePlayer> player_;
	// 次プレイヤーステート
	std::unique_ptr<BasePlayer> nextPlayer_ = nullptr;
	// プレイヤーステートファクトリー
	std::unique_ptr<AbstractPlayerStateFactory> playerStateFactory_ = nullptr;

	// プレイヤーステート間で引き継ぐ変数
	// 座標
	QuaternionTransform transform_;
	// ファイルのパス
	std::string filePath_;
	// レールカメラフラグ
	bool isRailCamera_;
	// カメラ
	Camera* camera_ = nullptr;
};

