#include "Bullet.h"
#include "CollisionManager.h"
#include "StageData.h"
#include "Object3dCommon.h"
#include "ImGuiManager.h"
#include "ScoreManager.h"
#include "ParticleManager.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void Bullet::Initialize(GameObject* parent, const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType)
{
	parent_ = parent;
	velocity_ = velocity;
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize("resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);

#endif // _DEBUG

	// コライダーを生成
	StageData::ColliderSpawnData colliders;
	colliders.center = Vector3{ 0.0f,0.0f,0.0f };
	colliders.size = transform.scale;
	colliders.parent = this;
	colliders.objectType = objectType;
	objectName_ = objectType;
	CollisionManager::GetInstance()->SetColliders(colliders);

	isDead_ = false;
}

void Bullet::Finalize()
{

}

void Bullet::Update()
{
	// 生存時間を更新
	currentTime_ += kDeltaTime;

	// 生存時間を過ぎたら削除
	if (currentTime_ >= kLifeTime_)
	{
		isDead_ = true;

		// 対応するエフェクトに死亡を通知
		ParticleManager::GetInstance()->SetBulletIsAlive(false, id_, particleName_);
	}

	// 座標を更新
	transform_.translate = Vector3Add(transform_.translate, velocity_);
	// 弾のローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

	// 対応するエフェクトに座標を送信
	ParticleManager::GetInstance()->SetBulletTranslate(transform_.translate, id_, particleName_);

#ifdef _DEBUG
	debugDraw->SetBoxTranslate(transform_.translate);
	debugDraw->UpdateBox();
#endif // _DEBUG

#ifdef USE_IMGUI
	ImGui::Begin("bullet");
	ImGui::DragFloat3("pos", &transform_.translate.x);

	ImGui::End();

#endif // USE_IMGUI

}

void Bullet::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG


}

void Bullet::OnCollision(const std::string& hitObjectType)
{
	if (objectName_ == "PlayerBulletSpawn")
	{
		// 当たったオブジェクトのタイプで分岐
		if (hitObjectType == "EnemySpawn")
		{
			// 敵だったらスコアを加算
			ScoreManager::GetInstance()->AddScore(10);
			ScoreManager::GetInstance()->SetNumber();
			// 弾の死亡フラグを立てる
			isDead_ = true;
			// 対応するエフェクトに死亡を通知
			ParticleManager::GetInstance()->SetBulletIsAlive(false, id_, particleName_);

		}
	}
	else if (objectName_ == "EnemyBulletSpawn")
	{
		// 当たったオブジェクトのタイプで分岐
		if (hitObjectType == "PlayerSpawn")
		{
			// 弾の死亡フラグを立てる
			isDead_ = true;
			// 対応するエフェクトに死亡を通知
			ParticleManager::GetInstance()->SetBulletIsAlive(false, id_, particleName_);
		}
	}


}
