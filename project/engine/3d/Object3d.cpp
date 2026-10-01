#include "Object3d.h" 
#include "Object3dCommon.h"
#include "MathManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include <numbers>
#include "TextureManager.h"
#include "Logger.h"

using namespace MathManager;

void Object3d::Initialize()
{
	// 引数で受け取ってメンバ変数として記録する
	this->object3dManager = Object3dCommon::GetInstance();
	dxBasis_ = object3dManager->GetDxBasis();


	// デフォルトカメラをセット
	this->camera = object3dManager->GetDefaultCamera();

	// 座標変換行列データ作成
	CreateTransformMatrixData3d();

	// 平行光源データ作成
	CreateDirectionalLight();

	// 点光源データ作成
	CreatePointLight();

	// スポットライトデータ作成
	CreateSpotLight();

	// カメラデータ作成
	CreateCameraResource();

	// おもちゃ風質感データ作成
	CreateToyTexture();

	// Transform変数を作る
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Object3d::CreateTransformMatrixData3d()
{
	// WVP用のリソースを作る
	transformationResource = dxBasis_->CreateBufferResources(sizeof(TransformationMatrix));
	// データを書き込む
	// 書き込むためのアドレスを取得
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	// 単位行列を書き込んでおく
	transformationData->World = MakeIdentity4x4();
	transformationData->WVP = MakeIdentity4x4();
	transformationData->WorldInverseTranspose = Inverse(transformationData->World);
	transformationData->WorldInverseTranspose = Transpose(transformationData->WorldInverseTranspose);
}

void Object3d::CreateDirectionalLight()
{
	// 平行光源用のリソースを作る
	directionalLightResource = dxBasis_->CreateBufferResources(sizeof(DirectionalLight));

	//書き込むためのアドレスを取得
	directionalLightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData->intensity = 0.5f;

}

void Object3d::CreatePointLight()
{
	// 点光源用のリソースを作る
	pointLightResource = dxBasis_->CreateBufferResources(sizeof(PointLight));
	// 書き込むためのアドレスを取得
	pointLightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&pointLightData));
	pointLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData->position = { 0.0f,2.0f,0.0f };
	pointLightData->intensity = 0.5f;
	pointLightData->radius = 100.0f;
	pointLightData->decay = 100.0f;
}

void Object3d::CreateSpotLight()
{
	// スポットライト用のリソースを作る
	spotLightResource = dxBasis_->CreateBufferResources(sizeof(SpotLight));
	// 書き込むためのアドレスを取得
	spotLightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&spotLightData));
	spotLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData->position = { 2.0f,1.25f,0.0f };
	spotLightData->distance = 7.0f;
	spotLightData->direction =
		Normalize({ -1.0f,-1.0f,0.0f });
	spotLightData->intensity = 4.0f;
	spotLightData->decay = 2.0f;
	spotLightData->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 3.0f);
}

void Object3d::CreateCameraResource()
{
	// カメラリソースの生成
	cameraResource = dxBasis_->CreateBufferResources(sizeof(CameraForGPU));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	cameraData_->worldPosition = camera->GetTranslate();

}

void Object3d::CreateToyTexture()
{
	// おもちゃ風質感用のリソースを作る
	toyTextureResource = dxBasis_->CreateBufferResources(sizeof(SpotLight));
	// 書き込むためのアドレスを取得
	toyTextureResource->Map(
		0, nullptr, reinterpret_cast<void**>(&toyTextureData));

	// データの初期値を設定
	// 緑っぽい輝き
	toyTextureData->rimLightPower = 4.0f;
	toyTextureData->rimLightIntensity = 0.25f;
	// 陰影
	toyTextureData->ambientStrength = 0.1f;
	// 彩度
	toyTextureData->saturation = 1.3f;
	// コントラスト
	toyTextureData->contrast = 1.15f;
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model = ModelManager::GetInstance()->FindModel(filePath);
}


void Object3d::Update()
{
	if (!isRailCamera_)
	{
		worldMatrix = MakeAffineMatrixQuat(transform.scale, transform.rotate, transform.translate);
	}

	// モデルを更新
	model->Update(worldMatrix);

	if (parent)
	{
		Matrix4x4 parentWorldMatrix = MakeAffineMatrixQuat(parent->GetScale(), parent->GetRotate(), parent->GetTranslate());
		worldMatrix = Multiply(worldMatrix, parentWorldMatrix);
	}

	if (camera)
	{
		viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else
	{
		worldViewProjectionMatrix = worldMatrix;
	}


	transformationData->WVP = worldViewProjectionMatrix;
	transformationData->World = worldMatrix;

	transformationData->WorldInverseTranspose = Transpose(Inverse(transformationData->World));
	if (camera)
	{
		cameraData_->worldPosition = camera->GetTranslate();
	}

#ifdef USE_IMGUI
	ImGui::Begin("SpotLight");
	ImGui::DragFloat3("pos", &spotLightData->position.x);
	ImGui::SliderFloat("intensity", &spotLightData->intensity, 0.0f, 10.0f);
	ImGui::SliderFloat("cosFalloffStart", &spotLightData->cosFalloffStart, 0.0f, 2.0f);
	ImGui::SliderFloat("cosAngle", &spotLightData->cosAngle, -1.0f, 1.0f);

	if (spotLightData->cosFalloffStart < spotLightData->cosAngle)
	{
		spotLightData->cosAngle = spotLightData->cosFalloffStart;
	}

	ImGui::End();

#endif // USE_IMGUI


}

void Object3d::Draw()
{
	// 3dモデルの描画準備
	Object3dCommon::GetInstance()->DrawSettingCommon();

	// wvp用のCBufferの場所を設定
	dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResource->GetGPUVirtualAddress());

	// モデルの質感タイプで分岐
	switch (object3dManager->GetTextureType())
	{
	case Object3dCommon::TextureType::kNormal:
		// 平行光源用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		// カメラリソース用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
		// 点光源用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource->GetGPUVirtualAddress());
		// スポットライト用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource->GetGPUVirtualAddress());
		// SRVのDescriptorTableの先頭を設定
		dxBasis_->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSRVHandleGPU(environmentMapTextureFilePath));


		break;

	case Object3dCommon::TextureType::kToy:
		// 平行光源用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
		// カメラリソース用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());
		// おもちゃ風質感リソース用のCBufferの場所を設定
		dxBasis_->GetCommandList()->SetGraphicsRootConstantBufferView(5, toyTextureResource->GetGPUVirtualAddress());

		break;

	}


	// 3Dモデルが割り当てられていれば描画する
	if (model)
	{
		model->Draw();
	}

}