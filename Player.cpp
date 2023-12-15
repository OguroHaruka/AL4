#include "Player.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);
	assert(models[0]);
	/*modelBody_ = models[0];
	modelHead_ = models[1];
	modelL_arm_ = models[2];
	modelR_arm_ = models[3];*/
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
	BaseCharacter::Update();
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
}

void Player::UpdateFloatingGimick() {
	const uint16_t period = uint32_t(90.0f);
	const float step = 2.0f * float(M_PI) / period;
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(M_PI));
	const float amplitube = 0.15f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitube;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformR_arm_, viewProjection);
	models_[3]->Draw(worldTransformL_arm_, viewProjection);
}

