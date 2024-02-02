#pragma once
#include "Model.h"
#include "Sprite.h"

class Fade {
public:
	void Initialize(float a);
	void Update(float b);
	void Draw();

private:
	Sprite* fadeSprite_;
	uint32_t fadeTexHandle;
	Vector4 fadeColor_;
};
