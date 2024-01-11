#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
class TitleScene {
public: 
	TitleScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }
	void SetIsSceneEnd(bool isSceneEnd_) { isSceneEnd = isSceneEnd_; }
	SceneType NextScene() { return SceneType::kGamePlay; }

	private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	bool isSceneEnd = false;

	Sprite* titleSprite_ = nullptr;
};
