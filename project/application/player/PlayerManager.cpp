#include "PlayerManager.h"
#include <cassert>

std::unique_ptr<PlayerManager> PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<PlayerManager>(ConstructorKey());
	}
	return instance.get();
}

void PlayerManager::SetPlayerData(const QuaternionTransform& transform, const std::string& filePath, bool isRailcamera, Camera* camera)
{
	// 前プレイヤーステートから渡されたデータをセット
	transform_ = transform;
	filePath_ = filePath;
	isRailCamera_ = isRailcamera;
	camera_ = camera;
}

void PlayerManager::ChangePlayerState(const std::string& playerState)
{
	assert(playerStateFactory_);
	assert(nextPlayer_ == nullptr);

	// 次のプレイヤーステートを生成
	nextPlayer_ = playerStateFactory_->CreatePlayerState(playerState);
}

void PlayerManager::Update()
{
	// プレイヤーステート切り替え

	// 次プレイヤーステートの予約があったら
	if (nextPlayer_)
	{
		// 旧プレイヤーステートを終了する
		if (player_)
		{
			player_->Finalize();
			player_.reset();
		}

		// プレイヤーステートを切り替える
		player_ = std::move(nextPlayer_);

		// 次プレイヤーステートを初期化する
		player_->Initialize(transform_,filePath_,isRailCamera_,camera_);
	}

	// 実行中プレイヤーステートを更新する
	player_->Update();
}

void PlayerManager::Draw()
{
	// 実行中プレイヤーステートを描画する
	player_->Draw();
}


void PlayerManager::Finalize()
{
	// 最後のプレイヤーステートの終了と解放
	player_->Finalize();
	player_.reset();
	instance.reset();
}
