#include "EnemyStateFactory.h"
#include "NormalEnemy.h"

std::unique_ptr<BaseEnemyState> EnemyStateFactory::CreateEnemyState(const std::string& enemyState)
{
    // 次の敵の状態を生成
	std::unique_ptr<BaseEnemyState> nextEnemyState;

	if (enemyState == "NormalEnemy")
	{
		nextEnemyState = std::make_unique<NormalEnemy>();
	}

	return nextEnemyState;
}
