#include <math.h>
#include <vector>

#include "KatamariBase.h"
#include "Game.h"
#include "InputDevice.h"
#include "WICTextureLoader.h"

KatamariBase::KatamariBase(
	Game* g,
	XMFLOAT3 objectPostition,
	Quaternion objectRotation,
	float radius
) : Object(
	g,
	objectPostition,
	objectRotation,
	"./Models/ToiletPapaer.obj",
	L"./Textures/Diffuse.png",
	1.00,
	radius,
	false
) {
	boundigSphere.Radius = radius;
	boundigSphere.Center = objectPostition;
	game->components.push_back(this);
}


void KatamariBase::Update(float deltaTime) {
	Object::Update(deltaTime);

	bSphere.Center = GetPosition();
	float speed = 5.0f;
	XMVECTOR vectorRight = game->camera.GetVectorRight();
	XMVECTOR vectorForward = XMVector3Cross(vectorRight, {0, 1,0 });

	if (game->inputDevice->IsKeyDown(Keys::W)) {
		XMStoreFloat3(&localPosition, XMLoadFloat3(&localPosition) + vectorForward * speed * deltaTime);
		SetRotation(GetRotation() * Quaternion::CreateFromAxisAngle(vectorRight, deltaTime * speed / boundigSphere.Radius));
	}

	if (game->inputDevice->IsKeyDown(Keys::S)) {
		XMStoreFloat3(&localPosition, XMLoadFloat3(&localPosition) - vectorForward * speed * deltaTime);
		SetRotation(GetRotation() * Quaternion::CreateFromAxisAngle(vectorRight, -deltaTime * speed / boundigSphere.Radius));
	}

	if (game->inputDevice->IsKeyDown(Keys::A)) {
		XMStoreFloat3(&localPosition, XMLoadFloat3(&localPosition) - vectorRight * speed * deltaTime);
		SetRotation(GetRotation() * Quaternion::CreateFromAxisAngle(vectorForward, deltaTime * speed / boundigSphere.Radius));
	}

	if (game->inputDevice->IsKeyDown(Keys::D)) {
		XMStoreFloat3(&localPosition, XMLoadFloat3(&localPosition) + vectorRight * speed * deltaTime);
		SetRotation(GetRotation() * Quaternion::CreateFromAxisAngle(vectorForward, -deltaTime * speed / boundigSphere.Radius));
	}

	if (shouldInter)
		Interpolate(0.02f);
}

void KatamariBase::UpdateBoundingSphere()
{
	boundigSphere.Radius += 0.29f;
	riseAmmount += 0.05f;
	shouldInter = true;
	bSphere.Radius += 0.5f;
}

void KatamariBase::Interpolate(float rate) {
	if (shouldInter) {
		float temp = (riseAmmount + localPosition.y - localPosition.y) * rate;
		localPosition.y += temp;
		riseAmmount -= temp;

		if (riseAmmount <= 0) {
			shouldInter = 0;
			riseAmmount = 0;
		}
	}
}
