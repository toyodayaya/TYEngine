#pragma once
#include "GameObject.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"
#endif // _DEBUG

class BaseBullet : public GameObject
{
public:
	// 初期化
	virtual void Initialize(const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType) = 0;
	// getter
 	uint32_t GetID() const { return id_; }
	// setter
	void SetID(uint32_t id) { id_ = id; }
	void SetParticleName(std::string name) { particleName_ = name; }

protected:
	
	// 移動速度
	Vector3 velocity_;
	// 生存時間
	float currentTime_ = 0;
	const float kLifeTime_ = 1;
	// エフェクト対応用のID
	uint32_t id_;
	// パーティクルグループの名前
	std::string particleName_;
	// オブジェクトタイプ
	std::string objectName_;
	
#ifdef _DEBUG
	std::unique_ptr<DebugDraw> debugDraw;
#endif // _DEBUG

};

