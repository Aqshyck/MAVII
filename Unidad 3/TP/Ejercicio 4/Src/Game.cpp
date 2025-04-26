#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física

	t.loadFromFile("Caja.png");
	s = new sf::Sprite(t);
	m_Avatar = new Avatar(controlBody, s);
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}


void Game::DrawGame()
{
	m_Avatar->Actualizar();
	m_Avatar->Dibujar(*wnd);	

	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujar las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	sf::RectangleShape topWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	topWallShape.setFillColor(sf::Color::Cyan);
	topWallShape.setPosition(50, 0);
	wnd->draw(topWallShape);
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt)) // Procesa todos los eventos acumulados
	{
		switch (evt.type)
		{
		case Event::Closed: // Si se solicita cerrar la ventana
			wnd->close(); // Cierra la ventana
			break;
		case Event::MouseButtonPressed: // Si se presiona un botón del ratón
			// Crea un cuerpo dinámico con forma triangular en la posición del clic
			b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y)); // Transforma la posición del clic a coordenadas del mundo
			body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f); // Mueve el cuerpo a la posición del clic
			break;
		}
	}
	// Controlar el movimiento del cuerpo de control con el teclado
   // Segun la numeracion usada, cuando mas cerca de cero mas 
   // lento es el desplazamiento sobre ese eje
	controlBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->ApplyForceToCenter(b2Vec2(-50.0f*controlBody->GetMass(), 0.0f), true);
	if (Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->ApplyForceToCenter(b2Vec2(50.0f * controlBody->GetMass(), 0.0f),true);
	if (Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->ApplyForceToCenter(b2Vec2(0.0f, 50.0f * controlBody->GetMass()),true);
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->ApplyForceToCenter(b2Vec2(0.0f, -50.0f * controlBody->GetMass()),true);
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes	
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Crear un rectangulo que se controlará con el teclado
	controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 10, 1.0f, 0.5, 0.1f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}