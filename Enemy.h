#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <math.h>
#include "ImGuiManager.h"
#include "BaseCharacter.h"

class Enemy : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	const ViewProjection* viewProjection_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	Enemy* enemy_ = nullptr;
	Input* input_ = nullptr;
	float floatingParameter_ = 0.0f;
};
