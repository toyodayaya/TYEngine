#include "PlayerStateFactory.h"
#include "NormalPlayer.h"

std::unique_ptr<BasePlayer> PlayerStateFactory::CreatePlayerState(const std::string& playerState)
{
    // 次のプレイヤーの状態を生成
	std::unique_ptr<BasePlayer> nextPlayerState;

	if(playerState == "NormalPlayer")
	{
		nextPlayerState = std::make_unique<NormalPlayer>();
	}

	return nextPlayerState;
}
