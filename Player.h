#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <math.h>
#include "ImGuiManager.h"
#include "BaseCharacter.h"

class Player : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>&models)override;
	void InitializeFloatingGimick();
	void Update()override;
	void UpdateFloatingGimick();
	void Draw(const ViewProjection& viewProjection)override;
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