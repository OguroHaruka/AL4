#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 3.0f, 0.0f};
}

void Player::Update() { 
	
	XINPUT_STATE joyState;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	const float kCharacterSpeed = 0.5f;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.z = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		move = Multiply(kCharacterSpeed, Normalize(move));

		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		
		move = TransformNormal(move, rotateYMatrix);
		
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		}
	
	if (move.z != 0 || move.y != 0) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.UpdateMatrix(); }

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}