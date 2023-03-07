#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include <memory>

using namespace DirectX::SimpleMath;

struct Transform
{
public:
	Transform() = default;
	Transform(Vector3 pos);
	~Transform() = default;

	Transform& getParent() const;
	std::vector<std::shared_ptr<Transform>>& getChild();
	void addChild(Transform* obj);
	void setParent(Transform* parent);

	Vector3 getPosition();
	void setPosition(Vector3 pos);
	void addPosition(Vector3 pos);

	std::unique_ptr<Transform> parent = nullptr;
	std::vector<std::shared_ptr<Transform>> child;

	Quaternion rotation;

	void rotate(Vector3 axis, float angle);

	Matrix CreateWorldMatrix() const;

private:
	Vector3 m_position;
	Matrix world;
	//float m_angle;
};

