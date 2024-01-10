#pragma once
#include "Model.h"
#include "WorldTransform.h"

class field {
public:
	void Initialize(Model* model,Vector3 pos,Vector3 scale);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPosition();
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

};
