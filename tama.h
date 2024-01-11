#pragma once
#include "Model.h"
#include "WorldTransform.h"
class tama {
public:
	void Initialize(Model* model, Vector3 pos, Vector3 scale);
	void Update();
	void Draw(ViewProjection& viewProjection);
	Vector3 GetWorldPosition();
	void SetMoveFlag(bool moveFlag_) { moveFlag = moveFlag_; }
	void SetTranslation(Vector3 translation) { worldTransform_.translation_ = translation; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	bool moveFlag;
	float speed;
};
