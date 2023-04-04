#include "Transform.h"

XMMATRIX Transform::GetView()  {
	return parent == nullptr ? GetLocalView() : parent->GetView() * GetLocalView();
}

XMMATRIX Transform::GetLocalView()  {
	Matrix localView =
		Matrix::CreateFromQuaternion(localRotation);
		localView = localView.Transpose();
		localView = Matrix::CreateTranslation(XMFLOAT3(-localPosition.x, -localPosition.y, -localPosition.z)) * localView;
		localView = localView * Matrix::CreateScale(1 / localScale.x, 1 / localScale.y, 1 / localScale.z);

	return localView;
}

XMMATRIX Transform::GetLocalObject() {
	return 
		Matrix::CreateScale(localScale) *
		Matrix::CreateFromQuaternion(localRotation) *
		Matrix::CreateTranslation(localPosition);
}

XMMATRIX Transform::GetObject()  {
	return parent == nullptr ? GetLocalObject() : GetLocalObject() * parent->GetObject();
}

XMVECTOR Transform::GetVectorRight() {
	Vector3 temp = { 1.0f, 0.0f, 0.0f };
	return XMVector3TransformNormal(temp, GetObject());
}

XMVECTOR Transform::GetVectorUp() {
	Vector3 temp = { 0.0f, 1.0f, 0.0f };
	return XMVector3TransformNormal(temp, GetObject());
}

XMVECTOR Transform::GetVectorForward() {
	Vector3 temp = { 0.0f, 0.0f, 1.0f };
	return XMVector3TransformNormal(temp, GetObject());
}


//Local
XMVECTOR Transform::GetVectorLocalRight() {
	Vector3 temp = { 1.0f, 0.0f, 0.0f };
	return XMVector3TransformNormal(temp, GetLocalObject());
}

XMVECTOR Transform::GetVectorLocalUp() {
	Vector3 temp = { 0.0f, 1.0f, 0.0f };
	return XMVector3TransformNormal(temp, GetLocalObject());
}

XMVECTOR Transform::GetVectorLocalForward() {
	Vector3 temp = { 0.0f, 0.0f, 1.0f };
	return XMVector3TransformNormal(temp, GetLocalObject());
}

XMFLOAT3 Transform::GetPosition() {
	XMVECTOR temp;

	if (!parent) 
		return localPosition;

	temp = XMLoadFloat3(&localPosition);
	temp = XMVector3TransformCoord(temp, parent->GetObject());
	XMFLOAT3 result;
	XMStoreFloat3(&result, temp);

	return result;
}

Quaternion Transform::GetRotation() {
	return parent == nullptr ? localRotation : localRotation * parent->GetRotation();
}

void Transform::SetPosition(const DirectX::XMFLOAT3& position) {
	if (!parent)
		localPosition = position;
	else  {
		XMVECTOR temp;
		temp = XMLoadFloat3(&position);
		temp = XMVector3TransformCoord(temp, parent->GetView());
		XMStoreFloat3(&localPosition, temp);
	}
}


void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& rotation) {
	if (!parent)
		localRotation = rotation;
	else {
		Quaternion temp;
		parent->GetRotation().Inverse(temp);
		localRotation = rotation * temp;
	}
}

void Transform::SetParent(Transform* p, bool saveValues) {
	if (saveValues)
		parent = p;
	else  {
		Quaternion tempRot = localRotation;
		XMFLOAT3 tempPos = localPosition;
		parent = p;
		SetPosition(tempPos);
		SetRotation(tempRot);
	}
}