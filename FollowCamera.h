#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"MyMath.h"
#include "Input.h"

class FollowCamera {
public:
	void Initialize();
	void Update();
	void SetTarget(const WorldTransform* target) { target_ = target; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;
};
