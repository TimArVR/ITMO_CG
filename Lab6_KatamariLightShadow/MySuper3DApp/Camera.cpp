#include "Camera.h"

Camera::Camera() {
	this->fov = DirectX::XM_PIDIV2;
	this->aspectRatio = 1;
	this->nearZ = 0.1f;
	this->farZ = 1000.0f;
	this->height = 80;
	this->isPerspective = true;
}

void Camera::SetProjectionValuesPerspective(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->fov = fovRadians;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;
}

void Camera::SetProjectionValuesOrthographic(float aspectRatio, float height, float nearZ, float farZ)
{
	this->aspectRatio = aspectRatio;
	this->height = height;
	this->nearZ = nearZ;
	this->farZ = farZ;
}

void Camera::SetIsPerspective(bool type)
{
	isPerspective = type;
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	if (isPerspective)
	{
		return DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
	}
	else 
	{
		return DirectX::XMMatrixOrthographicLH(aspectRatio * height, height, nearZ, farZ);
	}
}