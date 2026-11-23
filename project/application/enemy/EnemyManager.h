#pragma once
#include "BaseEnemy.h"
#include <memory>
#include <vector>

class EnemyManager
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class EnemyManager;
	};

	// PassKeyを受け取るコンストラクタ
	explicit EnemyManager(ConstructorKey) {}
	
private:
	// デストラクタ
	~EnemyManager() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	// インスタンス
	friend std::default_delete<EnemyManager>;
	static std::unique_ptr<EnemyManager> instance;


	// 登録済みの敵を記録する配列
	std::vector<std::unique_ptr<BaseEnemy>> enemies_;

	// パーティクルid
	uint32_t particleId_;
	// IDの最大値
	const uint32_t kMaxId = 128;

public:
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();

	// パーティクルid発行処理
	uint32_t AllocateID();
	
	// setter
	void SetEnemies(std::unique_ptr<BaseEnemy> enemy) { enemies_.push_back(std::move(enemy)); }
	// getter
	const std::vector<std::unique_ptr<BaseEnemy>>& GetEnemies() { return enemies_; }

	// インスタンス
	static EnemyManager* GetInstance();
};

