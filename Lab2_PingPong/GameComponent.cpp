#include ".\GameComponent.h"
#include "Game.h"

GameComponent::GameComponent(Game& game, DirectX::SimpleMath::Vector2 position)
: m_game(game), m_position(DirectX::SimpleMath::Vector4(position.x, position.y, 0.5f, 1.0f))
{
	
}

GameComponent::~GameComponent()
{
}
