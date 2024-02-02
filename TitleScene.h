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
#include "Fade.h"
#include "PlayerAnime.h"
#include "CameraAnime.h"
#include "Skydome.h"
#include "field.h"

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

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<PlayerAnime> playerAnime_;
	std::unique_ptr<Model> playerModel_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	std::unique_ptr<CameraAnime> cameraAnime_;

	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelHead_;
	std::unique_ptr<Model> modelL_arm_;
	std::unique_ptr<Model> modelR_arm_;
	std::unique_ptr<Model> modelL_asi_;
	std::unique_ptr<Model> modelR_asi_;

	std::unique_ptr<Model> modelHammer_;

	std::unique_ptr<Model> fieldModel_;
	std::unique_ptr<field> field_;

	bool isSceneEnd = false;

	Sprite* titleSprite_ = nullptr;
	std::unique_ptr<Fade> fade_;
	int fadeCount_;
	bool fadeFlag_;
};
