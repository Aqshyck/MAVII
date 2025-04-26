#pragma once
#include <Box2D/Box2D.h>
#include "Box2DHelper.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Config.hpp>
#include <stdlib.h>
using namespace sf; // Espacio de nombres para la biblioteca SFML



class Pelota
{
private:
	sf::Sprite* m_Sprite;
	b2Body* m_Body;
	bool confirmacion = true;

public:
	Pelota(b2Body* body, sf::Sprite* sprite);

	void Actualizar();
	void Dibujar(sf::RenderWindow& wnd);
};