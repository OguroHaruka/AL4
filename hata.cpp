#include "hata.h"

void hata::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 1.54f, 0.0f};
	worldTransform_.translation_ = {0.0f, -0.7f, 195.0f};
	}

void hata::Update() { worldTransform_.UpdateMatrix(); }

void hata::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

Vector3 hata::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
