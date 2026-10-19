#pragma once
#include "AbstractEnemyStateFactory.h"

class EnemyStateFactory : public AbstractEnemyStateFactory
{
public:
	// 敵の状態の生成
	std::unique_ptr<BaseEnemyState> CreateEnemyState(const std::string& enemyState) override;
};

