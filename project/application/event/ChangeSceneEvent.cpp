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
	// 判定用のオブジェクト
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());
	object3d_->SetModel("cube.obj");
	object3d_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	object3d_->SetTransform(transform);
	transform_ = transform;
	isDead_ = false;

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
	object3d_->Update();

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
