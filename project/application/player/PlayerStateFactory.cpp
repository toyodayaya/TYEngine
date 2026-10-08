#include "PlayerStateFactory.h"
#include "NormalPlayer.h"
#include "StandByPlayer.h"
#include "StartSequencePlayer.h"
#include "DeadPlayer.h"

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
	else if (playerState == "StartSequencePlayer")
	{
		nextPlayerState = std::make_unique<StartSequencePlayer>();
	}
	else if (playerState == "DeadPlayer")
	{
		nextPlayerState = std::make_unique<DeadPlayer>();
	}

	return nextPlayerState;
}
