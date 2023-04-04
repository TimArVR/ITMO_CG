#include "CollisionComponent.h"
#include "KatamariBase.h"
#include "Object.h"

bool CollisionComponent::CheckCollision() {
	if (!hasParent && target) {
		if (bSphere.Intersects(target->bSphere) && bSphere.Radius <= target->bSphere.Radius) {
			KatamariBase* pl = dynamic_cast<KatamariBase*>(target);

			pl->UpdateBoundingSphere();

			return true;
		}
	}

	return false;
}

void CollisionComponent::SetBoundingShape(XMFLOAT3 center, float radius, Object* target) {
	this->target = target;
	bSphere.Center = center;
	bSphere.Radius = radius;
}
