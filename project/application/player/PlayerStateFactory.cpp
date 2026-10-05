#include "PlayerStateFactory.h"
#include "NormalPlayer.h"
#include "StandByPlayer.h"

std::unique_ptr<BasePlayerState> PlayerStateFactory::CreatePlayerState(const std::string& playerState)
{
    // 次のプレイヤーの状態を生成
	std::unique_ptr<BasePlayerState> nextPlayerState;

	if(playerState == "NormalPlayer")
	{
		nextPlayerState = std::make_unique<NormalPlayer>();
	}
	else if (playerState == "StandByPlayer")
	{
		nextPlayerState = std::make_unique<StandByPlayer>();
	}

	return nextPlayerState;
}
