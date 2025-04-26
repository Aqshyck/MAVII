#include "Pelota.h"

Pelota::Pelota(b2Body* body, sf::Sprite* sprite) : m_Body(body), m_Sprite(sprite)
{
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

	for (b2Fixture* f = m_Body->GetFixtureList(); f; f = f->GetNext())
	{
		aabb.Combine(f->GetAABB(0));
	}
	float sizeX = aabb.GetExtents().x * 2.0 / m_Sprite->getTexture()->getSize().x;	//Obtengo escala del objeto en X
	float sizeY = aabb.GetExtents().y * 2.0 / m_Sprite->getTexture()->getSize().y;	//Obtengo escala del objeto en Y
	m_Sprite->setScale(sizeX, sizeY);
	m_Sprite->setOrigin(sf::Vector2f(m_Sprite->getTexture()->getSize().x / 2.0f, m_Sprite->getTexture()->getSize().y / 2.0f));
	b2Vec2 pos = m_Body->GetPosition();
	m_Sprite->setPosition(sf::Vector2f(pos.x, pos.y));
}

void Pelota::Actualizar()
{
	b2Vec2 pos = m_Body->GetPosition();					//Obtenemos la posicion de la forma fisica
	m_Sprite->setPosition(sf::Vector2f(pos.x, pos.y));	//Pasamos la posicion al Sprite
	float ang = m_Body->GetAngle();						//Obtenemos el angulo de la forma fisica (esta en radianes)
	m_Sprite->setRotation(ang * 180 / 3.1415f);			//Pasamos los radianes a grados
}

void Pelota::Dibujar(sf::RenderWindow& wnd)
{
	wnd.draw(*m_Sprite);
}