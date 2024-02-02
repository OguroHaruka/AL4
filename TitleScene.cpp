#include "TitleScene.h"

TitleScene::TitleScene() {}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	uint32_t textureTitle = TextureManager::Load("Title.png");
	titleSprite_ =
	    Sprite::Create(textureTitle, {-300.0f, -160.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	fade_ = std::make_unique<Fade>();
	fade_->Initialize(0.0f);
	fadeCount_ = 0;
	fadeFlag_ = false;
}

void TitleScene::Update() { 
	
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
