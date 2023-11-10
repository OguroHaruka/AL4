#include "FollowCamera.h"

void FollowCamera::Initialize() { 
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

void FollowCamera::Update() {
	XINPUT_STATE joyState;
	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		offset = TransformNormal(offset, MakeRotateYMatrix(viewProjection_.rotation_.y)); // 途中

		viewProjection_.translation_ = Add(target_->translation_,offset);
	}

	const float kCameraSpeed = 0.05f;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		viewProjection_.rotation_.y+=(float)joyState.Gamepad.sThumbRX / SHRT_MAX* kCameraSpeed;
	}
	viewProjection_.UpdateMatrix();
	
}