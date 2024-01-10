#include "Player.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);
	assert(models[0]);
	/*modelBody_ = models[0];
	modelHead_ = models[1];
	modelL_arm_ = models[2];
	modelR_arm_ = models[3];*/
	isHammerDraw_ = false;
	isHammerSet_ = false;
	r_asi_animeflag = false;
	l_asi_animeflag = false;
	fallSpeed_ = 0.0f;
	fallFlag_ = true;
	stanbyTime = 0;
	InitializeFloatingGimick();
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 10.0f, 0.0f};
	worldTransformBody_.Initialize();
	worldTransformBody_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformBody_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
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
	worldTransformHammer_.Initialize();
	worldTransformHammer_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHammer_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHammer_.translation_ = {0.0f, 1.0f, 0.0f};
	worldTransformR_asi_.Initialize();
	worldTransformR_asi_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformR_asi_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_asi_.translation_ = {-0.25f, 0.6f, 0.0f};
	worldTransformL_asi_.Initialize();
	worldTransformL_asi_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformL_asi_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformL_asi_.translation_ = {0.25f, 0.6f, 0.0f};
}

void Player::InitializeFloatingGimick() { 
	floatingParameter_ = 0.0f;
}

void Player::Update() { 
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) { 
			case Behavior::kRoot:
		default:
			    BehaviorRootInitialize();
			    break;
		case Behavior::kAttack:
			    BehaviorAttackInitialize();
			    
			    break;
		}
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;
	worldTransformR_asi_.parent_ = &worldTransformBody_;
	worldTransformL_asi_.parent_ = &worldTransformBody_;

	BaseCharacter::Update();
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_asi_.UpdateMatrix();
	worldTransformL_asi_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::UpdateFloatingGimick() {
	const uint16_t period = uint32_t(90.0f);
	const float step = 2.0f * float(M_PI) / period;
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * float(M_PI));
	const float amplitube = 0.15f;
	//worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitube;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * amplitube;
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformR_arm_, viewProjection);
	models_[3]->Draw(worldTransformL_arm_, viewProjection);
	models_[5]->Draw(worldTransformR_asi_, viewProjection);
	models_[6]->Draw(worldTransformL_asi_, viewProjection);
	if (isHammerDraw_ == true) {
		models_[4]->Draw(worldTransformHammer_, viewProjection);
	}
}

void Player::BehaviorRootUpdate() {
	BehaviorRootInitialize();
	XINPUT_STATE joyState;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	const float kCharacterSpeed = 0.3f;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.z = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		move = Multiply(kCharacterSpeed, Normalize(move));

		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);

		move = TransformNormal(move, rotateYMatrix);

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0) {
			    if (l_asi_animeflag == false) {
				    worldTransformL_asi_.rotation_.x += 0.1f;
				    if (worldTransformL_asi_.rotation_.x >= 1.0f) {
					    l_asi_animeflag = true;
				    }
			    } else if (l_asi_animeflag == true) {
				    worldTransformL_asi_.rotation_.x -= 0.1f;
				    if (worldTransformL_asi_.rotation_.x <= -1.0f) {
					    l_asi_animeflag = false;
				    }
			    }
			    if (r_asi_animeflag == false) {
				    worldTransformR_asi_.rotation_.x -= 0.1f;
				    if (worldTransformR_asi_.rotation_.x <= -1.0f) {
					    r_asi_animeflag = true;
				    }
			    } else if (r_asi_animeflag == true) {
				    worldTransformR_asi_.rotation_.x += 0.1f;
				    if (worldTransformR_asi_.rotation_.x >= 1.0f) {
					    r_asi_animeflag = false;
				    }
			    } 
		} else {
			    worldTransformL_asi_.rotation_.x = 0.0f;
			    worldTransformR_asi_.rotation_.x = 0.0f;
			    l_asi_animeflag = false;
			    r_asi_animeflag = false;
		}
	}

	
	if (fallSpeed_ >= 4.0f) {
		fallSpeed_ = 4.0f;
	}
	if (fallFlag_ == true) {
		fallSpeed_ += 0.05f;
		worldTransform_.translation_.y -= fallSpeed_;
	}
	if (worldTransform_.translation_.y <= -100.0f) {
		worldTransform_.translation_ = {0.0f, 50.0f, 0.0f};
	}
	if (move.z != 0 || move.y != 0) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	UpdateFloatingGimick();
}

void Player::BehaviorAttackUpdate() { 
	
	worldTransformL_arm_.rotation_.x =
	    worldTransformHammer_.rotation_.x+3.08f;
	worldTransformR_arm_.rotation_.x =
	    worldTransformHammer_.rotation_.x+3.08f;
	if (isHammerSet_ == false) {
		if (worldTransformHammer_.rotation_.x >= -0.4f) {
			worldTransformHammer_.rotation_.x -= 0.05f;
		}
		if (worldTransformHammer_.rotation_.x <= -0.4f) {
			worldTransformHammer_.rotation_.x = -0.4f;
			stanbyTime++;
			if (stanbyTime >= 12) {
				isHammerSet_ = true;
				stanbyTime = 0;
			}
		}
	} else if (isHammerSet_ == true) {
		if (worldTransformHammer_.rotation_.x <= 1.6f) {
			worldTransformHammer_.rotation_.x += 0.1f;
		}
		if (worldTransformHammer_.rotation_.x >= 1.6f) {
			worldTransformHammer_.rotation_.x = 1.6f;
			stanbyTime++;
			if (stanbyTime >= 12) {
				isHammerSet_ = false;
				behaviorRequest_ = Behavior::kRoot;
				stanbyTime = 0;
			}
		}
	}
}

void Player::BehaviorRootInitialize() { isHammerDraw_ = false;
	worldTransformHammer_.rotation_.x = 0.0f;
}

void Player::BehaviorAttackInitialize() { isHammerDraw_ = true; }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

