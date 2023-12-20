#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <math.h>
#include "ImGuiManager.h"
#include "BaseCharacter.h"
#include <optional>

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
	void BehaviorRootUpdate();
	void BehaviorAttackUpdate();
	void BehaviorRootInitialize();
	void BehaviorAttackInitialize();
	enum class Behavior { kRoot, kAttack, };

private:
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;
	const ViewProjection* viewProjection_ = nullptr; 
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;
	float floatingParameter_ = 0.0f;
	bool isHammerDraw_;
	bool isHammerSet_;
	int stanbyTime;
};