#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {  }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	

	player_ = std::make_unique<Player>();
	modelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));
	std::vector<Model*> playerModels = {
	    modelBody_.get(), modelHead_.get(), modelL_arm_.get(), modelR_arm_.get(),modelHammer_.get()};
	player_->Initialize(playerModels);

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());

	skydome_ = std::make_unique<Skydome>();
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));
	ground_->Initialize(groundModel_.get());

	enemy_ = std::make_unique<Enemy>();
	modelNeedleBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelNeedleL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	modelNeedleR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));
	std::vector<Model*> enemyModels = {
	    modelNeedleBody_.get(), modelNeedleL_arm_.get(), modelNeedleR_arm_.get()};
	enemy_->Initialize(enemyModels);

	}

void GameScene::Update() { 
	player_->Update();
	enemy_->Update();
	debugCamera_->Update();
	followCamera_->Update();
	
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActive_ = false;
	}
#endif
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.TransferMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
