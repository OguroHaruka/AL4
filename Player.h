#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <math.h>
#include "ImGuiManager.h"

class Player {
public:
	void Initialize(Model* modelBody,Model*modelHead,Model*modelL_arm,Model*modelR_arm);
	void InitializeFloatingGimick();
	void Update();
	void UpdateFloatingGimick();
	void Draw(ViewProjection& viewProjection);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	const ViewProjection* viewProjection_ = nullptr; 
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;
	float floatingParameter_ = 0.0f;
};