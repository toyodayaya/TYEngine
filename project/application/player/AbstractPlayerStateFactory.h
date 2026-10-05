#pragma once
#include "BasePlayerState.h"
#include <string>
#include <memory>

class AbstractPlayerStateFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractPlayerStateFactory() = default;
	// プレイヤーの状態の生成
	virtual std::unique_ptr<BasePlayerState> CreatePlayerState(const std::string& playerState) = 0;
};