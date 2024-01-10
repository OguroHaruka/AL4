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
#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "field.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

private: // メンバ変数

	void CheakCollisions();

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	int isDebugCameraActive_;
	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<Player> player_;
	uint32_t playerTexture_ = 0;
	std::unique_ptr<Model> playerModel_;
	
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelHead_;
	std::unique_ptr<Model> modelL_arm_;
	std::unique_ptr<Model> modelR_arm_;
	std::unique_ptr<Model> modelL_asi_;
	std::unique_ptr<Model> modelR_asi_;

	std::unique_ptr<Model> modelHammer_;

	std::unique_ptr <Model> fieldModel_;
	std::unique_ptr<field> field_;

	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyModel_;

	std::unique_ptr<Model> modelNeedleBody_;
	std::unique_ptr<Model> modelNeedleL_arm_;
	std::unique_ptr<Model> modelNeedleR_arm_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
