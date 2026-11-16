#include "NormalPlayer.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "BulletManager.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include "ParticleManager.h"
#include <cmath>

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void NormalPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	camera_ = player_->GetCamera();
	hp_ = player_->GetHP();
	//emitter_ = player_->GetEmitter();

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(true);
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };
	transform_.translate = Vector3Add(transform_.translate, playerOffset_);
	isHit_ = false;

	// 3Dレティクルオブジェクトの初期化
	reticle_ = player_->GetReticle();
	reticleTransform_ = transform_;
	reticleTransform_.translate = Vector3Add(reticleTransform_.translate, reticleOffset_);

	// ロックオンマークを初期化
	lockOn_ = player_->GetLockOn();

	// UIスプライトの初期化
	inGameUI_ = player_->GetInGameUI();
	inGameUI_->SetCanFireBeamPoint(kCanFireBeamPoint);
	inGameUI_->SetMaxBeamPoint(kMaxBeamPoint);
	inGameUI_->SetMaxBeamPointSize(kMaxBeamPoint);
	inGameUI_->SetCurrentBeamPoint(currentBeamPoint_);
	inGameUI_->SetBeamPointSize(currentBeamPoint_);

	// パーティクル初期値を初期化
	emitterSphere.velocity = { 0.0f,0.0f,0.0f };
	emitterSphere.type = ParticleManager::MoveType::kFollow;
	emitterSphere.scale = { 1.0f,1.0f,1.0f };
	emitterSphere.lifeTime = 3.0f;
	emitterSphere.frequency = 0.0f;
	emitterSphere.frequencyTime = 0.0f;
	emitterSphere.currentTime = 0;
	emitterSphere.count = 1;
	emitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };

	traceEmitterSphere.translate = GetWorldTranslate();
	traceEmitterSphere.id = 0;
	traceEmitterSphere.velocity = { 0.0f,0.0f,0.0f };
	traceEmitterSphere.type = ParticleManager::MoveType::kNone;
	traceEmitterSphere.scale = { 1.0f,1.0f,1.0f };
	traceEmitterSphere.lifeTime = 1.5f;
	traceEmitterSphere.frequency = 0.1f;
	traceEmitterSphere.frequencyTime = 0.0f;
	traceEmitterSphere.currentTime = 0;
	traceEmitterSphere.count = 1;
	traceEmitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	traceEmitter = std::make_unique <ParticleEmitter>("Trace", traceEmitterSphere);

	flashEmitterSphere.translate = GetWorldTranslate();
	flashEmitterSphere.id = 0;
	flashEmitterSphere.velocity = { 0.0f,0.0f,0.0f };
	flashEmitterSphere.type = ParticleManager::MoveType::kDiffusion;
	flashEmitterSphere.scale = { 1.0f,1.0f,1.0f };
	flashEmitterSphere.lifeTime = 0.5f;
	flashEmitterSphere.frequency = 0.2f;
	flashEmitterSphere.frequencyTime = 0.0f;
	flashEmitterSphere.currentTime = 0;
	flashEmitterSphere.count = 8;
	flashEmitterSphere.color = { 1.0f,1.0f,1.0f,1.0f };
	flashEmitter = std::make_unique <ParticleEmitter>("Flash", flashEmitterSphere);
}

void NormalPlayer::Update()
{
	// ヒットフラグが立っていたら
	if (isHit_)
	{
		// ヒットタイマーを更新する
		HitTimerUpdate();
	}

	// プレイヤーの攻撃処理
	Attack();

	// プレイヤーの移動処理
	Move();

	// 3Dレティクルを更新
	UpdateReticle();

	// ロックオンマークを更新
	lockOn_->Update();

	// スプライトの更新
	inGameUI_->Update();

	// エミッターの更新
	traceEmitter->SetTranslate(GetWorldTranslate());
	traceEmitter->Update();

	if (currentBeamPoint_ >= kCanFireBeamPoint)
	{
		// 発射可能エフェクトを更新
		flashEmitter->SetTranslate(GetWorldTranslate());
		flashEmitter->Update();
	}
	
#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &transform_.translate.x);
	ImGui::DragFloat4("rotate", &transform_.rotate.x);
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::DragFloat3("target", &targetPosition.x);
	ImGui::DragFloat3("reticlePos", &reticleTransform_.translate.x);
	ImGui::End();

#endif // USE_IMGUI


}

void NormalPlayer::Draw()
{
	// ヒットフラグが立っていたら
	if (isHit_)
	{
		// 点滅させる
		if (hitTimer_ % 2 == 0.0f)
		{
			object3d_->Draw();
		}
	}
	else
	{
		// オブジェクトを描画する
		object3d_->Draw();
	}

	// ロックオンマークの描画
	lockOn_->Draw();

	// UIの描画
	inGameUI_->Draw();
}

void NormalPlayer::Finalize()
{}

void NormalPlayer::OnCollision(std::string hitObjectType)
{
	if (isHit_)
	{
		// 既に当たっていたら処理しない
		return;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "BulletSpawn")
	{
		// 自弾だったらスキップ
		return;
	}
	else if (hitObjectType == "EnemySpawn")
	{
		// HPを減らす
		hp_--;

		// HPが0になったら
		if (hp_ <= 0)
		{
			// ステートを変更する
			PlayerManager::GetInstance()->ChangePlayerState("DeadPlayer");
			hp_ = 0;
		}

		inGameUI_->SetHpSize(hp_);
		player_->SetHP(hp_);
	}


	// ヒットフラグを立てる
	isHit_ = true;

}

void NormalPlayer::CreateBullet()
{

	if (lockOn_->GetTarget())
	{
		targetPosition = lockOn_->GetTarget()->GetWorldTranslate();
	}
	else
	{
		targetPosition = GetReticleWorldTranslate();
	}

	// 弾の初期座標を設定
	QuaternionTransform bulletTransform = transform_;
	bulletTransform.translate = GetWorldTranslate();

	// 速度を算出
	velocity = Vector3Subtract(targetPosition, bulletTransform.translate);
	velocity = Normalize(velocity);
	velocity = FloatMultiply(velocity, kBeamSpeed_);

	// 生成と初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize(bulletTransform, filePath_, velocity, false);
	BulletManager::GetInstance()->SetBulletID(bullet.get());

	// エフェクトを追加
	emitterSphere.translate = bulletTransform.translate;
	emitterSphere.id = bullet->GetID();
	emitter = std::make_unique <ParticleEmitter>("Bullet", emitterSphere);
	// エフェクトを発生させる
	emitter->Update();
	// 対応するエフェクトに生存フラグを通知
	ParticleManager::GetInstance()->SetBulletIsAlive(true, bullet->GetID(), "Bullet");
	// パーティクルグループの名前を記録
	bullet->SetParticleName("Bullet");

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(bullet));

	// 発射間隔タイマーをリセット
	bulletIntervalTimer_ = 10;

}

void NormalPlayer::CreateBeam()
{
	// ビーム弾数をカウント
	currentBeamBulletNum++;

	if (lockOn_->GetTarget())
	{
		targetPosition = lockOn_->GetTarget()->GetWorldTranslate();
	}
	else
	{
		targetPosition = GetReticleWorldTranslate();
	}

	// ビームの初期座標を設定
	QuaternionTransform bulletTransform = transform_;
	bulletTransform.translate = GetWorldTranslate();

	// 速度を算出
	velocity = Vector3Subtract(targetPosition, bulletTransform.translate);
	velocity = Normalize(velocity);
	velocity = FloatMultiply(velocity, kBulletSpeed_);

	// 生成と初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize(bulletTransform, filePath_, velocity, false);

	// エフェクトを追加
	emitterSphere.translate = bulletTransform.translate;
	emitterSphere.id = bullet->GetID();
	emitter = std::make_unique <ParticleEmitter>("Beam", emitterSphere);
	// エフェクトを発生させる
	emitter->Update();
	// 対応するエフェクトに生存フラグを通知
	ParticleManager::GetInstance()->SetBulletIsAlive(true, bullet->GetID(), "Beam");
	// パーティクルグループの名前を記録
	bullet->SetParticleName("Beam");

	// BulletManagerに追加
	BulletManager::GetInstance()->SetBullets(std::move(bullet));

	// 弾数上限に達したら
	if (currentBeamBulletNum >= kBeamBulletNum)
	{
		// ビーム発射フラグを外す
		canFireBeam_ = false;
		// 弾数カウントをリセット
		currentBeamBulletNum = 0;
	}
}

void NormalPlayer::UpdateReticle()
{
	// 自機のワールド行列の回転を適用
	Matrix4x4 world = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	reticleOffset_ = TransformNormal(reticleOffset_, world);
	// ベクトルの長さを整える
	reticleOffset_ = FloatMultiply(Normalize(reticleOffset_), kDistance_);
	// 3Dレティクルの位置を決定
	reticleTransform_.translate = Vector3Add(transform_.translate, reticleOffset_);
	reticle_->SetTransform(reticleTransform_);

	// 3Dオブジェクトの更新
	reticleWorldMatrix = MakeAffineMatrixQuat(reticleTransform_.scale, reticleTransform_.rotate, reticleTransform_.translate);
	reticleWorldMatrix = Multiply(reticleWorldMatrix, camera_->GetWorldMatrix());
	reticle_->SetWorldMatrix(reticleWorldMatrix);
	reticle_->Update();

	// スプライトのレティクルに座標設定
	lockOn_->LockOnTarget(reticle_);

#ifdef USE_IMGUI
	ImGui::Begin("Reticle");
	ImGui::DragFloat3("pos", &reticleTransform_.translate.x);

	ImGui::End();

#endif // USE_IMGUI


}

void NormalPlayer::Move()
{
	// キー入力でプレイヤーを移動させる
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		transform_.translate.x -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_D))
	{
		transform_.translate.x += 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_S))
	{
		transform_.translate.y -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_W))
	{
		transform_.translate.y += 0.1f;
	}

	// 範囲を超えない処理
	transform_.translate.x = max(transform_.translate.x, -kMoveLimitX_);
	transform_.translate.x = std::min(transform_.translate.x, +kMoveLimitX_);
	transform_.translate.y = max(transform_.translate.y, -kMoveLimitY_);
	transform_.translate.y = std::min(transform_.translate.y, +kMoveLimitY_);


	// 座標を更新
	object3d_->SetTranslate(transform_.translate);

	// プレイヤーのローカル行列を作成
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);
	// カメラのワールド行列を乗算
	worldMatrix = Multiply(worldMatrix, camera_->GetWorldMatrix());
	// プレイヤーのワールド座標をセット
	object3d_->SetWorldMatrix(worldMatrix);
	player_->SetWorldMatrix(worldMatrix);

	// 3Dオブジェクトを更新
	object3d_->Update();
}

void NormalPlayer::Attack()
{
	// ビーム発射フラグが立っていたら
	if (canFireBeam_)
	{
		// ビーム発射間隔が経過したら
		if (beamFireInterval_ <= 0)
		{
			// 弾数分ビームを生成する
			CreateBeam();
			// 間隔をリセット
			beamFireInterval_ = 3;
		}
		else
		{
			beamFireInterval_--;
		}

		
	}
	else
	{
		// キー入力で弾を生成
		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			CreateBullet();
		}
		else if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			if (bulletIntervalTimer_ <= 0)
			{
				// 弾の発射間隔タイマーが0になったら発射			
				CreateBullet();
			}
			else
			{
				// タイマーを減らす
				bulletIntervalTimer_--;
			}

		}
		else if (Input::GetInstance()->TriggerKey(DIK_RETURN))
		{
			// ポイントが貯まっていたら
			if (currentBeamPoint_ >= kCanFireBeamPoint)
			{
				// ビーム発射可能フラグを立てる
				canFireBeam_ = true;
				// ビームポイントをリセット
				currentBeamPoint_ = 0;
				inGameUI_->SetCurrentBeamPoint(currentBeamPoint_);
				inGameUI_->SetBeamPointSize(currentBeamPoint_);
			}
		}
	}

}

void NormalPlayer::HitTimerUpdate()
{
	// ヒットタイマーが0になったら
	if (hitTimer_ <= 0)
	{
		// フラグを元に戻す
		isHit_ = false;
		hitTimer_ = 60;
	}
	else
	{
		// ヒットタイマーを減らす
		hitTimer_--;
	}
}

void NormalPlayer::SetBeamPoint(int point)
{
	// 引数で受け取ったポイントを加算
	currentBeamPoint_ += point;

	// ポイント上限を超えたらカンスト
	if (kMaxBeamPoint <= currentBeamPoint_)
	{
		currentBeamPoint_ = kMaxBeamPoint;
	}

	inGameUI_->SetCurrentBeamPoint(currentBeamPoint_);
	inGameUI_->SetBeamPointSize(currentBeamPoint_);
}
