#include "Player.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	assert(modelBody);
	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;
	InitializeFloatingGimick();
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.Initialize();
	worldTransformBody_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformBody_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 3.0f, 0.0f};
	worldTransformHead_.Initialize();
	worldTransformHead_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHead_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_arm_.translation_ = {-0.5f, 1.2f, 0.0f};
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformL_arm_.translation_ = {0.5f, 1.2f, 0.0f};
	
}

void Player::InitializeFloatingGimick() { 
	floatingParameter_ = 0.0f;
}

void Player::Update() { 
	
	XINPUT_STATE joyState;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	const float kCharacterSpeed = 0.5f;

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;

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

	UpdateFloatingGimick();



	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
}

void Player::UpdateFloatingGimick() {
	const uint16_t period = uint32_t(60.0f);
	const float step = 2.0f * float(M_PI) / period;
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(M_PI));
	const float amplitube = 0.15f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitube;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
}

void Player::Draw(ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformR_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformL_arm_, viewProjection);
}

