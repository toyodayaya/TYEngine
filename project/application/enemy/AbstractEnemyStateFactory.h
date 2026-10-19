#pragma once
#include "BaseEnemyState.h"
#include <string>
#include <memory>

class AbstractEnemyStateFactory
{
public:
	// 仮想デストラクタ
	virtual ~AbstractEnemyStateFactory() = default;
	// 敵の状態の生成
	virtual std::unique_ptr<BaseEnemyState> CreateEnemyState(const std::string& enemyState) = 0;
};