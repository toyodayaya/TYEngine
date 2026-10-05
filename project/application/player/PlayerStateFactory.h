#pragma once
#include "AbstractPlayerStateFactory.h"

class PlayerStateFactory : public AbstractPlayerStateFactory
{
public:
	// プレイヤーの状態の生成
	std::unique_ptr<BasePlayerState> CreatePlayerState(const std::string& playerState) override;
};

