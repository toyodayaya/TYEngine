#pragma once
#include "GameObject.h"

class BaseEvent : public GameObject
{
public:
	// 初期化
	virtual void Initialize(const QuaternionTransform& transform) = 0;
	
private:

};

