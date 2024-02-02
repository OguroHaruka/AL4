#include "CameraAnime.h"

void CameraAnime::Initialize() { viewProjection_.Initialize(); }

void CameraAnime::Update() {
	
	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		offset = TransformNormal(offset, MakeRotateYMatrix(viewProjection_.rotation_.y)); 

		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	viewProjection_.rotation_.y += 0.005f;

	
	viewProjection_.UpdateMatrix();
}