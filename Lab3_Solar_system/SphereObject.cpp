#include "Game.h"
#include "SphereObject.h"
#include "ConstantBuffer.h"

using namespace DirectX;
using namespace SimpleMath;
float rand_FloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}


SphereObject::SphereObject(Game* game, Vector3 position, Vector4 color, float radius, float boxSpeed)
	: GameObject(game, position, color), m_boxSize(radius), m_boxSpeed(boxSpeed)
{
	constexpr int sliceCount = 16;
	constexpr int stackCount = 16;

		Vertex topVertex(Vector3(0.0f, +radius, 0.0f) /*+ position*/, color);
		Vertex bottomVertex(Vector3(0.0f, -radius, 0.0f) /*+ position*/, color);

		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f * XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.pos.x = radius * sinf(phi) * cosf(theta) /*+ position.x*/;
				v.pos.y = radius * cosf(phi) /*+ position.y*/;
				v.pos.z = radius * sinf(phi) * sinf(theta) /*+ position.z*/;

				v.color = { rand_FloatRange(0,1),rand_FloatRange(0,1),rand_FloatRange(0,1),1 };

				vertices.push_back(v);
			}
		}

		vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}
		indicesCount = std::size(indices);

		init(vertices, indices);
}

void SphereObject::preDraw()
{
	m_rotateDelta += static_cast<float>(DirectX::XM_PI) * m_game->deltaTime * m_boxSpeed;
	transform->rotate(Vector3::UnitY, m_rotateDelta);

	// Update Constant Buffer
	const ConstantBuffer cb =
	{
		transform->CreateWorldMatrix().Transpose(),
		m_game->camera->getViewMatrix().Transpose(),
		m_game->camera->getProjectionMatrix().Transpose(),
	};
	m_game->context->UpdateSubresource(pConstantBuffer.Get(), 0, NULL, &cb, 0, 0);
}

