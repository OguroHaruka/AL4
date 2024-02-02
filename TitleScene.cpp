#include "TitleScene.h"

TitleScene::TitleScene() {}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	playerAnime_ = std::make_unique<PlayerAnime>();
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

	std::vector<Model*> playerModels = {modelBody_.get(),  modelHead_.get(),   modelL_arm_.get(),
	                                    modelR_arm_.get(), modelHammer_.get(), modelL_asi_.get(),
	                                    modelR_asi_.get()};
	playerAnime_->Initialize(playerModels);

	cameraAnime_ = std::make_unique<CameraAnime>();
	cameraAnime_->Initialize();
	cameraAnime_->SetTarget(&playerAnime_->GetWorldTransform());

	playerAnime_->SetViewProjection(&cameraAnime_->GetViewProjection());

	skydome_ = std::make_unique<Skydome>();
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(skydomeModel_.get());

	uint32_t textureTitle = TextureManager::Load("Title.png");
	titleSprite_ =
	    Sprite::Create(textureTitle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	fade_ = std::make_unique<Fade>();
	fade_->Initialize(0.0f);
	fadeCount_ = 0;
	fadeFlag_ = false;

}

void TitleScene::Update() { 
	playerAnime_->Update();
	cameraAnime_->Update();
	field_->Update();
	viewProjection_.matProjection = cameraAnime_->GetViewProjection().matProjection;
	viewProjection_.matView = cameraAnime_->GetViewProjection().matView;
	viewProjection_.TransferMatrix();
	if (fadeFlag_ == true) {
		fadeCount_++;
		fade_->Update(0.01f);
	}
	if (fadeCount_ >= 120) {
		fadeCount_ = 0;
		fadeFlag_ = false;
		fade_->Initialize(0.0f);
		isSceneEnd = true;
	}
	XINPUT_STATE joyState; 
if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
		fadeFlag_ = true;
		}
	}

}

void TitleScene::Draw() {// コマンドリストの取得
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

	playerAnime_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	field_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	
	titleSprite_->Draw();
	if (fadeCount_ >= 1) {
		fade_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
