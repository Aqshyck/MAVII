#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana y configuraci�n de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuraci�n de la vista del juego
    InitPhysics(); // Inicializaci�n del motor de f�sica

}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulaci�n f�sica
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualizaci�n de la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo f�sico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo f�sico para depuraci�n
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    sf::Texture t;
    t.loadFromFile("Pelota.png");
    sf::Sprite* s = new sf::Sprite(t);
    m_AvatarUno = new Avatar(circuloUno, s);
    m_AvatarDos = new Avatar(circuloDos, s);

    m_AvatarUno->Actualizar();
    m_AvatarUno->Dibujar(*wnd);  
    m_AvatarDos->Actualizar();
    m_AvatarDos->Dibujar(*wnd);

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
            wnd->close();
            break;
        case Event::MouseButtonPressed:
            if (evt.mouseButton.button == sf::Mouse::Left)
            {
                Vector2f mouseWorldPos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
                b2Body* selectedBody = nullptr;

                if (m_AvatarUno->Posicion(mouseWorldPos, circuloUno))
                {
                    selectedBody = circuloUno;
                }
                else if (m_AvatarDos->Posicion(mouseWorldPos, circuloDos))
                {
                    selectedBody = circuloDos;
                }

                if (selectedBody && !mouseJoint)
                {
                    mouseJoint = Box2DHelper::CreateMouseJoint(phyWorld, selectedBody, 1000.0f* selectedBody->GetMass(), 100000.0f, 100.0f);
                    mouseJoint->SetTarget(b2Vec2(mouseWorldPos.x, mouseWorldPos.y));                    
                    selectedBody->SetAwake(true);
                }
            }                
            break;
        case Event::MouseButtonReleased:
            if (evt.mouseButton.button == sf::Mouse::Left && mouseJoint) 
            {
                phyWorld->DestroyJoint(mouseJoint);  
                mouseJoint = nullptr; 
            }
                
            break;
        case Event::MouseMoved:
            if (mouseJoint)
            {
                Vector2f mouseWorldPos = wnd->mapPixelToCoords(Vector2i(evt.mouseMove.x, evt.mouseMove.y)); 
                mouseJoint->SetTarget(b2Vec2(mouseWorldPos.x, mouseWorldPos.y)); 
            }
            break;
        }
    }
}

// Comprobaci�n de colisiones (a implementar m�s adelante)
void Game::CheckCollitions()
{
    // Implementaci�n de la comprobaci�n de colisiones
}

// Configuraci�n de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tama�o de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
    // Inicializar el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo f�sico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes est�ticas del mundo f�sico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    leftWallBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

    b2Body* roofBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
    roofBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    // Crear el primer c�rculo
    circuloUno = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.8, 2.0f);
    circuloUno->SetTransform(b2Vec2(30.0f, 50.0f), 0.0f);

    // Crear el segundo c�rculo
    circuloDos = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.8, 1.0f);
    circuloDos->SetTransform(b2Vec2(70.0f, 50.0f), 0.0f);

    // Atamos las esferas mediante un resorte
    Box2DHelper::CreateDistanceJoint(phyWorld, circuloUno, circuloUno->GetWorldCenter(),
        circuloDos, circuloDos->GetWorldCenter(), 5.0f, 0.7f, 0.8f);

    b2MouseJoint* mouseJoint = nullptr;

}

// Destructor de la clase

Game::~Game(void)
{ }