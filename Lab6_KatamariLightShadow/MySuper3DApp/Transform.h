#pragma once
#include "DisplayWin32.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Transform {
public:
	Transform* parent = nullptr;
	XMFLOAT3 localPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 localScale = { 1.0f, 1.0f, 1.0f };
	Quaternion localRotation = Quaternion::Identity;

	XMMATRIX GetView();
	XMMATRIX GetObject();
	XMMATRIX GetLocalObject();
	XMMATRIX GetLocalView();

	void SetParent(Transform* p, bool saveValues);

	XMFLOAT3 GetPosition();
	void SetPosition(const XMFLOAT3& position);

	Quaternion GetRotation();
	void SetRotation(const Quaternion& rotation);

	XMVECTOR GetVectorRight();
	XMVECTOR GetVectorUp();
	XMVECTOR GetVectorForward();
	XMVECTOR GetVectorLocalRight();
	XMVECTOR GetVectorLocalUp();
	XMVECTOR GetVectorLocalForward();
};