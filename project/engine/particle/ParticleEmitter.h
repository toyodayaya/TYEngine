
#pragma once
#include <string>
#include <MathManager.h>
#include <ParticleManager.h>
#include <wrl.h>

class ParticleEmitter
{
public:
	// インスタンス
	friend std::default_delete<ParticleEmitter>;
	static std::unique_ptr<ParticleEmitter> instance;

	// コンストラクタ
	ParticleEmitter(const std::string name,const ParticleManager::EmitterSphere& emitterSphere);
	// デストラクタ
	~ParticleEmitter() = default;
	// コピーコンストラクタとコピー代入演算子を削除
	ParticleEmitter(ParticleEmitter&) = delete;
	ParticleEmitter& operator=(ParticleEmitter&) = delete;

private:

	// メンバ変数
	std::string name;
	ParticleManager::EmitterSphere emitter{};

public:

	// パーティクル発生
	void Emit();

	// 更新処理
	void Update();

	// setter
	void SetTranslate(const Vector3& pos) { emitter.translate = pos; }
	void SetEmitterSphere(const ParticleManager::EmitterSphere& emitters) { emitter = emitters; }
};
