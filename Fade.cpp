
#include "Fade.h"

void Fade::Initialize(float a) {
	fadeTexHandle = TextureManager::Load("BLIND.png");
	fadeColor_ = {1.0f, 1.0f, 1.0f, a};
	fadeSprite_ = Sprite::Create(fadeTexHandle, {0, 0});
}

void Fade::Update(float b) {
	fadeColor_.w += b;
	fadeSprite_->SetColor(fadeColor_);
}
void Fade::Draw() { fadeSprite_->Draw(); }
