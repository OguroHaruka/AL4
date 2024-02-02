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
	/*AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);*/

	player_ = std::make_unique<Player>();
	modelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelL_asi_.reset(Model::CreateFromOBJ("float_L_asi", true));
	modelR_asi_.reset(Model::CreateFromOBJ("float_R_asi", true));
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));
	fieldModel_.reset(Model::CreateFromOBJ("field", true));

	field_ = std::make_unique<field>();
	field_->Initialize(fieldModel_.get(), {5.0f, -2.5f, 5.0f}, {5.0f, 1.1f, 5.0f});

	moveYuka_ = std::make_unique<MoveYuka>();
	moveYuka_->Initialize(fieldModel_.get(), {4.0f, -2.5f, 13.0f}, {4.0f, 1.1f, 4.0f});

	field2_ = std::make_unique<field>();
	field2_->Initialize(fieldModel_.get(), {5.0f, -2.5f, 200.0f}, {5.0f, 1.1f, 5.0f});

	hataModel_.reset(Model::CreateFromOBJ("hata", true));
	hata_ = std::make_unique<hata>();
	hata_->Initialize(hataModel_.get());

	tamaModel_.reset(Model::CreateFromOBJ("tama", true));
	for (int i=0; i < 10; i++) {
		tama_[i] = std::make_unique<tama>();
	}
	tama_[0]->Initialize(tamaModel_.get(), {0.0f, 1.0f, 250.0f}, {2.0f, 2.0f, 2.0f});
	tama_[1]->Initialize(tamaModel_.get(), {3.0f, 1.0f, 260.0f}, {2.0f, 2.0f, 2.0f});
	tama_[2]->Initialize(tamaModel_.get(), {-3.0f, 1.0f, 280.0f}, {2.0f, 2.0f, 2.0f});
	tama_[3]->Initialize(tamaModel_.get(), {0.0f, 1.0f, 285.0f}, {2.0f, 2.0f, 2.0f});
	tama_[4]->Initialize(tamaModel_.get(), {3.0f, 1.0f, 300.0f}, {2.0f, 2.0f, 2.0f});
	tama_[5]->Initialize(tamaModel_.get(), {-3.0f, 1.0f, 300.0f}, {2.0f, 2.0f, 2.0f});
	tama_[6]->Initialize(tamaModel_.get(), {-3.0f, 1.0f, 370.0f}, {2.0f, 2.0f, 2.0f});
	tama_[7]->Initialize(tamaModel_.get(), {0.0f, 1.0f, 375.0f}, {2.0f, 2.0f, 2.0f});
	tama_[8]->Initialize(tamaModel_.get(), {3.0f, 1.0f, 390.0f}, {2.0f, 2.0f, 2.0f});
	tama_[9]->Initialize(tamaModel_.get(), {0.0f, 1.0f, 395.0f}, {2.0f, 2.0f, 2.0f});

	std::vector<Model*> playerModels = {
	    modelBody_.get(), modelHead_.get(), modelL_arm_.get(), modelR_arm_.get(),modelHammer_.get(),modelL_asi_.get(),modelR_asi_.get()};
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

	count = 0;

	fade_ = std::make_unique<Fade>();
	fade_->Initialize(1.0f);

	}

void GameScene::Update() { 
	fade_->Update(-0.005f);
	player_->Update();
	//enemy_->Update();
	debugCamera_->Update();
	followCamera_->Update();
	field_->Update();
	field2_->Update();
	moveYuka_->Update();
	hata_->Update();
	tama_[0]->Update();
	tama_[1]->Update();
	tama_[2]->Update();
	tama_[3]->Update();
	tama_[4]->Update();
	tama_[5]->Update();
	tama_[6]->Update();
	tama_[7]->Update();
	tama_[8]->Update();
	tama_[9]->Update();
	CheakCollisions();
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
	//enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	field_->Draw(viewProjection_);
	field2_->Draw(viewProjection_);
	moveYuka_->Draw(viewProjection_);
	hata_->Draw(viewProjection_);
	tama_[0]->Draw(viewProjection_);
	tama_[1]->Draw(viewProjection_);
	tama_[2]->Draw(viewProjection_);
	tama_[3]->Draw(viewProjection_);
	tama_[4]->Draw(viewProjection_);
	tama_[5]->Draw(viewProjection_);
	tama_[6]->Draw(viewProjection_);
	tama_[7]->Draw(viewProjection_);
	tama_[8]->Draw(viewProjection_);
	tama_[9]->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	fade_->Draw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheakCollisions() {
	count++;
	Vector3 posA, posB,posC,posD,posE[10],posF;
	float posAE[10];
	float posAF;
	posA = player_->GetWorldPosition();
	posB = field_->GetWorldPosition();
	posC = moveYuka_->GetWorldPosition();
	posD = field2_->GetWorldPosition();
	posF = hata_->GetWorldPosition();

	posAF = (posF.x - posA.x) * (posF.x - posA.x) + (posF.y - posA.y) * (posF.y - posA.y) +
	        (posF.z - posA.z) * (posF.z - posA.z);
	if (posAF <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		isSceneEnd = true;
		
	}

	posE[0] = tama_[0]->GetWorldPosition();
	posE[1] = tama_[1]->GetWorldPosition();
	posE[2] = tama_[2]->GetWorldPosition();
	posE[3] = tama_[3]->GetWorldPosition();
	posE[4] = tama_[4]->GetWorldPosition();
	posE[5] = tama_[5]->GetWorldPosition();
	posE[6] = tama_[6]->GetWorldPosition();
	posE[7] = tama_[7]->GetWorldPosition();
	posE[8] = tama_[8]->GetWorldPosition();
	posE[9] = tama_[9]->GetWorldPosition();

	posAE[0] = (posE[0].x - posA.x) * (posE[0].x - posA.x) +
		       (posE[0].y - posA.y) * (posE[0].y - posA.y) +
		       (posE[0].z - posA.z) * (posE[0].z - posA.z);
	if (posAE[0] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[1] = (posE[1].x - posA.x) * (posE[1].x - posA.x) +
	           (posE[1].y - posA.y) * (posE[1].y - posA.y) +
	           (posE[1].z - posA.z) * (posE[1].z - posA.z);
	if (posAE[1] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[2] = (posE[2].x - posA.x) * (posE[2].x - posA.x) +
	           (posE[2].y - posA.y) * (posE[2].y - posA.y) +
	           (posE[2].z - posA.z) * (posE[2].z - posA.z);
	if (posAE[2] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[3] = (posE[3].x - posA.x) * (posE[3].x - posA.x) +
	           (posE[3].y - posA.y) * (posE[3].y - posA.y) +
	           (posE[3].z - posA.z) * (posE[3].z - posA.z);
	if (posAE[3] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[4] = (posE[4].x - posA.x) * (posE[4].x - posA.x) +
	           (posE[4].y - posA.y) * (posE[4].y - posA.y) +
	           (posE[4].z - posA.z) * (posE[4].z - posA.z);
	if (posAE[4] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[5] = (posE[5].x - posA.x) * (posE[5].x - posA.x) +
	           (posE[5].y - posA.y) * (posE[5].y - posA.y) +
	           (posE[5].z - posA.z) * (posE[5].z - posA.z);
	if (posAE[5] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[6] = (posE[6].x - posA.x) * (posE[6].x - posA.x) +
	           (posE[6].y - posA.y) * (posE[6].y - posA.y) +
	           (posE[6].z - posA.z) * (posE[6].z - posA.z);
	if (posAE[6] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[7] = (posE[7].x - posA.x) * (posE[7].x - posA.x) +
	           (posE[7].y - posA.y) * (posE[7].y - posA.y) +
	           (posE[7].z - posA.z) * (posE[7].z - posA.z);
	if (posAE[7] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[8] = (posE[8].x - posA.x) * (posE[8].x - posA.x) +
	           (posE[8].y - posA.y) * (posE[8].y - posA.y) +
	           (posE[8].z - posA.z) * (posE[8].z - posA.z);
	if (posAE[8] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	posAE[9] = (posE[9].x - posA.x) * (posE[9].x - posA.x) +
	           (posE[9].y - posA.y) * (posE[9].y - posA.y) +
	           (posE[9].z - posA.z) * (posE[9].z - posA.z);
	if (posAE[9] <= (1.0f + 2.0f) * (1.0f, 2.0f)) {
		Reset();
	}
	
	if (posA.y <= -100.0f) {
		Reset();
	}
	
	if (posB.y + 1.0f >= posA.y - 2.0f &&posB.y<=posA.y+0.5f&& 
		posB.z >= posA.z - 0.3f && posB.z-10.0f <= posA.z + 0.4f&&
		posB.x>=posA.x-0.6&&posB.x-10.0f<=posA.x+0.6f) {
		player_->SetFallSpeed(0.0f);
		player_->SetFallFlag(false);
		player_->SetTranslationY(0.5f);
	} else {
		player_->SetFallFlag(true);
	}

	if (posC.x>=posA.x-0.6f&&posC.x-8.0f<=posA.x+0.6f&&
		posC.y+1.0f>=posA.y-2.0f&&posC.y<=posA.y+0.5f&& 
		posC.z >= posA.z - 0.3f&&posC.z - 8.0f <= posA.z + 0.4f) {
		if (posC.z -4.0f <= posA.z) {
			if (count>=3) {
				moveYuka_->SetMoveFlag(true);
			}
			tama_[0]->SetMoveFlag(true);
			tama_[1]->SetMoveFlag(true);
			tama_[2]->SetMoveFlag(true);
			tama_[3]->SetMoveFlag(true);
			tama_[4]->SetMoveFlag(true);
			tama_[5]->SetMoveFlag(true);
			tama_[6]->SetMoveFlag(true);
			tama_[7]->SetMoveFlag(true);
			tama_[8]->SetMoveFlag(true);
			tama_[9]->SetMoveFlag(true);
		}
		player_->SetFallSpeed(0.0f);
		player_->SetFallFlag(false);
		player_->SetTranslationY(0.5f);
	} if (
	    posC.z+0.4 >= posA.z - 0.3f && posC.z < posA.z - 0.3f && posA.y <= 1.49f &&posA.y+2.5f>=-0.99f&&
	    player_->GetFallFlag() == true) {
		player_->SetTranslationZ(posC.z+0.8f);
	}
	if (posC.z  -8.4 <= posA.z + 0.4f && posC.z-8.0f > posA.z + 0.4f && posA.y <= 1.49f &&
	    posA.y + 2.5f >= -0.99f && player_->GetFallFlag() == true) {
		player_->SetTranslationZ(posC.z - 8.9f);
	}
	if (posC.x - 8.4f <= posA.x + 0.6f && posC.x - 8.0f > posA.x + 0.6f && posA.y <= 1.49f &&
	    posA.y + 2.5f >= -0.99f && player_->GetFallFlag() == true) {
		player_->SetTranslationX(posC.x -9.0f);
	}
	if (posC.x + 0.4f >= posA.x - 0.6f  && posC.x < posA.x - 0.6f && posA.y <= 1.49f &&
	    posA.y + 2.5f >= -0.99f && player_->GetFallFlag() == true) {
		player_->SetTranslationX(posC.x + 1.8f);
	}

	if (posD.y + 1.0f >= posA.y - 2.0f && posD.y <= posA.y + 0.5f && posD.z >= posA.z - 0.3f &&
	    posD.z - 10.0f <= posA.z + 0.4f && posD.x >= posA.x - 0.6 &&
	    posD.x - 10.0f <= posA.x + 0.6f) {
		player_->SetFallSpeed(0.0f);
		player_->SetFallFlag(false);
		player_->SetTranslationY(0.5f);
	}
}

void GameScene::Reset() {
	count = 0;
	player_->SetTranslation({0.0f, 50.0f, 0.0f});
	moveYuka_->SetTranslation({4.0f, -2.5f, 13.0f});
	moveYuka_->SetMoveFlag(false);
	for (int i=0; i < 10; i++) {
		tama_[i]->SetMoveFlag(false);
	}
	tama_[0]->SetTranslation({0.0f, 1.0f, 250.0f});
	tama_[1]->SetTranslation({3.0f, 1.0f, 260.0f});
	tama_[2]->SetTranslation({-3.0f, 1.0f, 280.0f});
	tama_[3]->SetTranslation({0.0f, 1.0f, 285.0f});
	tama_[4]->SetTranslation({3.0f, 1.0f,300.0f});
	tama_[5]->SetTranslation({-3.0f, 1.0f, 300.0f});
	tama_[6]->SetTranslation({-3.0f, 1.0f, 370.0f});
	tama_[7]->SetTranslation({0.0f, 1.0f, 375.0f});
	tama_[8]->SetTranslation({3.0f, 1.0f, 390.0f});
	tama_[9]->SetTranslation({0.0f, 1.0f, 395.0f});
}

void GameScene::FadeReset() { fade_->Initialize(1.0f); }
