#include "Gas.h"
#include "CollisionManager.h"
#include "StageData.h"
#include "Object3dCommon.h"
#include "ImGuiManager.h"
#include "ScoreManager.h"
#include "ParticleManager.h"
#include "PlayerManager.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void Gas::Initialize(const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType)
{
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	Vector3 size = transform.scale;
	size.x *= 10.0f;
	size.y *= 10.0f;
	debugDraw->SetBoxScale(size);
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);

#endif // _DEBUG

	// コライダーを生成
	StageData::ColliderSpawnData colliders;
	colliders.center = Vector3{ 0.0f,0.0f,0.0f };
	colliders.size = transform.scale;
	colliders.size.x *= 10.0f;
	colliders.size.y *= 10.0f;
	colliders.parent = this;
	colliders.objectType = objectType;
	objectName_ = objectType;
	CollisionManager::GetInstance()->SetColliders(colliders);

	isDead_ = false;
}

void Gas::Finalize()
{

}

void Gas::Update()
{
	// 弾のローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);


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

void Gas::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG


}

void Gas::OnCollision(std::string hitObjectType)
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
			// スリップダメージを付与する
			//PlayerManager::GetInstance()->SetPlayerHP(1);
		}
	}


}
