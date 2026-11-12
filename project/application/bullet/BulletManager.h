#pragma once
#include "BaseBullet.h"
#include <memory>
#include <vector>

class BulletManager
{
public:
	// コンストラクタに渡すための鍵
	class ConstructorKey
	{
	private:
		ConstructorKey() = default;
		friend class BulletManager;
	};

	// PassKeyを受け取るコンストラクタ
	explicit BulletManager(ConstructorKey) {}
	
private:
	// デストラクタ
	~BulletManager() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	BulletManager(const BulletManager&) = delete;
	BulletManager& operator=(const BulletManager&) = delete;
	// インスタンス
	friend std::default_delete<BulletManager>;
	static std::unique_ptr<BulletManager> instance;


	// 登録済みの敵を記録する配列
	std::vector<std::unique_ptr<BaseBullet>> bullets_;
	// 各弾に割り当てるID
	uint32_t nextId_ = 0;
	// IDの最大値
	const uint32_t kMaxId = 64;

public:
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();


	// setter
	void SetBullets(std::unique_ptr<BaseBullet> bullet) { bullets_.push_back(std::move(bullet)); }
	void SetBulletID(BaseBullet* bullet);

	// インスタンス
	static BulletManager* GetInstance();
};

