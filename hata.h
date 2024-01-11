#pragma once
#include "Model.h"
#include "WorldTransform.h"
class hata {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	bool moveFlag;
	float speed;
};
