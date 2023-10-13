#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
};