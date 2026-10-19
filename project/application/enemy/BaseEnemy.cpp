#include "BaseEnemy.h"
#include <cassert>

void BaseEnemy::ChangeEnemyState(const std::string& enemyState)
{
	assert(enemyStateFactory_);
	assert(nextEnemyState_ == nullptr);

	// 次のプレイヤーステートを生成
	nextEnemyState_ = enemyStateFactory_->CreateEnemyState(enemyState);
	// プレイヤーステート名を記録
	enemyStateName_ = enemyState;
}

void BaseEnemy::ChangeEnemyState(std::unique_ptr<BaseEnemyState> nextState)
{
	// 旧プレイヤーステートを終了する
	if (state_)
	{
		state_->Finalize();
		state_.reset();
	}

	// プレイヤーステートを切り替える
	state_ = std::move(nextState);

	// 次プレイヤーステートを初期化する
	state_->Initialize(this);
}
