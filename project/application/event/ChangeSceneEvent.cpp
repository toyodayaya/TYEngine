#include "ChangeSceneEvent.h"
#include "SceneManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void ChangeSceneEvent::Initialize(const QuaternionTransform& transform)
{
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxRotate(transform.rotate);
	debugDraw->SetBoxTranslate(transform.translate);
#endif // _DEBUG
}

void ChangeSceneEvent::Finalize()
{
#ifdef _DEBUG
	debugDraw.reset();
#endif // _DEBUG
}

void ChangeSceneEvent::Update()
{
	
#ifdef _DEBUG
	// デバッグ描画の更新処理
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void ChangeSceneEvent::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void ChangeSceneEvent::OnCollision()
{
	// プレイヤーが接触したらリザルトシーンへ
	SceneManager::GetInstance()->ChangeScene("ResultScene");
}
