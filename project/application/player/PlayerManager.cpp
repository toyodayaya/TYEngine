#include "PlayerManager.h"
#include <cassert>
#include "NormalPlayer.h"

std::unique_ptr<PlayerManager> PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<PlayerManager>(ConstructorKey());
	}
	return instance.get();
}

void PlayerManager::SetBeamPoint(int point)
{
	// プレイヤーステートが通常の時のみ
	if (playerStateName_ == "NormalPlayer")
	{
		// 引数で受け取ったポイントを加算
		NormalPlayer* normalPlayer = dynamic_cast<NormalPlayer*>(player_->GetPlayerState());
		normalPlayer->SetBeamPoint(point);
	}
}

void PlayerManager::Initialize(const QuaternionTransform& transform, const std::string filePath, Camera* camera)
{
	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(transform, filePath, camera);
}

void PlayerManager::ChangePlayerState(const std::string& playerState)
{
	assert(playerStateFactory_);
	assert(nextPlayerState_ == nullptr);

	// 次のプレイヤーステートを生成
	nextPlayerState_ = playerStateFactory_->CreatePlayerState(playerState);
	// プレイヤーステート名を記録
	playerStateName_ = playerState;
}

void PlayerManager::Update()
{
	// プレイヤーステート切り替え

	// 次プレイヤーステートの予約があったら
	if (nextPlayerState_)
	{
		// ステートを変更する
		player_->ChangePlayerState(std::move(nextPlayerState_));
	}

	// プレイヤーを更新する
	player_->Update();
}

void PlayerManager::Draw()
{
	// プレイヤーを描画する
	player_->Draw();
}


void PlayerManager::Finalize()
{
	// 最後のプレイヤーステートの終了と解放
	player_->Finalize();
	player_.reset();
	instance.reset();
}
