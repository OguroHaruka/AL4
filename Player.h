#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"

class Player {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr; 
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;
};