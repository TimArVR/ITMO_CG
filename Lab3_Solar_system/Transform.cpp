#include "Transform.h"

Transform::Transform(Vector3 pos) : m_position(pos)
{
}

Transform& Transform::getParent() const
{
	return *parent;
}

std::vector<std::shared_ptr<Transform>>& Transform::getChild()
{
	return child;
}

void Transform::addChild(Transform* obj)
{
	child.push_back(std::unique_ptr<Transform>(obj));
}

void Transform::setParent(Transform* p)
{
	parent = std::unique_ptr<Transform>(p);
}

Vector3 Transform::getPosition()
{
	return m_position;
}

void Transform::setPosition(Vector3 pos)
{
	m_position = pos;
}

void Transform::addPosition(Vector3 pos)
{
	m_position += pos;
}

void Transform::rotate(const Vector3 axis, const float angle)
{
	rotation = Quaternion::CreateFromYawPitchRoll(axis.y * angle, axis.x * angle, axis.z * angle);
}

Matrix Transform::CreateWorldMatrix() const
{
	Matrix mat = Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(m_position) * Matrix::CreateFromQuaternion(rotation);

	if (parent)
	{
		auto parMat = parent->CreateWorldMatrix();
		mat = mat * Matrix::CreateTranslation(parMat.Translation());
	}

	return mat;
}
