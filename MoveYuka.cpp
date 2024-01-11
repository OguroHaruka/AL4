#include "MoveYuka.h"

void MoveYuka::Initialize(Model* model, Vector3 pos, Vector3 scale) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = scale;
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = pos;
	moveFlag = false;
}

void MoveYuka::Update() { 
	if (moveFlag == true) {
		worldTransform_.translation_.z += 0.15f;
	}
	if (worldTransform_.translation_.z >= 190.0f) {
		worldTransform_.translation_.z = 190.0f;
	}
	worldTransform_.UpdateMatrix(); }

void MoveYuka::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 MoveYuka::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
