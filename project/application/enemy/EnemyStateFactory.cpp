#include "EnemyStateFactory.h"
#include "NormalEnemy.h"
#include "StandByEnemy.h"
#include "RushEnemy.h"
#include "SnipeEnemy.h"
#include "ExplosionEnemy.h"
#include "GasEnemy.h"

std::unique_ptr<BaseEnemyState> EnemyStateFactory::CreateEnemyState(const std::string& enemyState)
{
    // 次の敵の状態を生成
	std::unique_ptr<BaseEnemyState> nextEnemyState;

	if (enemyState == "NormalEnemy")
	{
		nextEnemyState = std::make_unique<NormalEnemy>();
	}
	else if (enemyState == "StandByEnemy")
	{
		nextEnemyState = std::make_unique<StandByEnemy>();
	}
	else if (enemyState == "RushEnemy")
	{
		nextEnemyState = std::make_unique<RushEnemy>();
	}
	else if (enemyState == "SnipeEnemy")
	{
		nextEnemyState = std::make_unique<SnipeEnemy>();
	}
	else if (enemyState == "ExplosionEnemy")
	{
		nextEnemyState = std::make_unique<ExplosionEnemy>();
	}
	else if (enemyState == "GasEnemy")
	{
		nextEnemyState = std::make_unique<GasEnemy>();
	}


	return nextEnemyState;
}
