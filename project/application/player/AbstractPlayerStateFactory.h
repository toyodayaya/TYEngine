#pragma once
#include "BasePlayer.h"
#include <string>
#include <memory>

class AbstractPlayerStateFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractPlayerStateFactory() = default;
	// プレイヤーの状態の生成
	virtual std::unique_ptr<BasePlayer> CreatePlayerState(const std::string& playerState) = 0;
};