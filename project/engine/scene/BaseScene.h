#pragma once

class BaseScene
{
public:
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// 終了
	virtual void Finalize() = 0;

	// 仮想デストラクタ
	virtual ~BaseScene() = default;

	// setter
	void SetIsSceneChange(bool isChangeScene) { isChangeScene_ = isChangeScene; }

protected:
	// シーン遷移フラグ
	bool isChangeScene_ = false;
	
};

