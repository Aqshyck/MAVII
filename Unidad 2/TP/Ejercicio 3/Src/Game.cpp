#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Dibujar las paredes
    sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    leftWallShape.setFillColor(sf::Color::Blue);
    leftWallShape.setPosition(0, 0); // X = 100 para que comience donde termina el suelo
    wnd->draw(leftWallShape);

    sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    rightWallShape.setFillColor(sf::Color::Blue);
    rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
    wnd->draw(rightWallShape);

    sf::RectangleShape roofShape(sf::Vector2f(500, 5));
    roofShape.setFillColor(sf::Color::Cyan);
    roofShape.setPosition(0, 95);
    wnd->draw(roofShape);

    // Dibujar los obstaculos

    sf::RectangleShape obstacleShape1(sf::Vector2f(5, 15)); // Alto de la ventana
    obstacleShape1.setFillColor(sf::Color::Blue);
    obstacleShape1.setPosition(60, 80); // X = 90 para que comience donde termina el suelo
    obstacleShape1.setOrigin(2.5, 7.5);
    obstacleShape1.setRotation(30);
    wnd->draw(obstacleShape1);

    sf::RectangleShape obstacleShape2(sf::Vector2f(5, 15)); // Alto de la ventana
    obstacleShape2.setFillColor(sf::Color::Blue);
    obstacleShape2.setPosition(80, 20); // X = 90 para que comience donde termina el suelo
    obstacleShape2.setOrigin(2.5, 7.5);
    obstacleShape2.setRotation(45);
    wnd->draw(obstacleShape2);

    sf::RectangleShape obstacleShape3(sf::Vector2f(5, 15)); // Alto de la ventana
    obstacleShape3.setFillColor(sf::Color::Blue);
    obstacleShape3.setPosition(30, 80); // X = 90 para que comience donde termina el suelo
    obstacleShape3.setOrigin(2.5, 7.5);
    obstacleShape3.setRotation(140);
    wnd->draw(obstacleShape3);

    // Dibujar el cuerpo de control (círculo)

    sf::CircleShape controlShape(5);
    controlShape.setFillColor(sf::Color::Magenta);
    controlShape.setPosition(controlBody->GetPosition().x - 5, controlBody->GetPosition().y - 5);
    wnd->draw(controlShape);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }

    // Controlar el movimiento del cuerpo de control con el teclado
    // Segun la numeracion usada, cuando mas cerca de cero mas 
    // lento es el desplazamiento sobre ese eje
    controlBody->SetAwake(true);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Down))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
    if (Keyboard::isKeyPressed(Keyboard::Up))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes estáticas del mundo físico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    leftWallBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    b2Body* roofBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
    roofBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    // Crear obstaculos

    b2Body* obstacleBody1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 15);
    obstacleBody1->SetTransform(b2Vec2(60.0f, 80.0f), 0.5236f);

    b2Body* obstacleBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 15);
    obstacleBody2->SetTransform(b2Vec2(80.0f, 20.0f), 0.7854f);

    b2Body* obstacleBody3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 5, 15);
    obstacleBody3->SetTransform(b2Vec2(30.0f, 80.0f), 2.4434f);

    // Crear un círculo que se controlará con el teclado
    controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.8, 2.0f);
    controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{ }