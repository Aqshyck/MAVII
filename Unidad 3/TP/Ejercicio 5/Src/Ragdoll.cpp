#include "Ragdoll.h"
#include <iostream>

Ragdoll::Ragdoll(b2World* world, const b2Vec2& position)
{
    // Definicion cuerpo
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = false;

    // Seteado inicial de posiciones
    // Torso   
    m_Body = world->CreateBody(&bodyDef);
    m_Body->SetTransform(position,b2_pi);
    std::cout << "Torso position: (" << m_Body->GetPosition().x << ", " << m_Body->GetPosition().y << ")\n";

    
    // Cabeza
    m_Head = world->CreateBody(&bodyDef);
    m_Head->SetTransform(position+b2Vec2(0.0f,-7.0F), b2_pi);
    std::cout << "Head position: (" << m_Head->GetPosition().x << ", " << m_Head->GetPosition().y << ")\n";

    // Brazo derecho
    m_RightArm = world->CreateBody(&bodyDef);
    m_RightArm->SetTransform(position + b2Vec2(4.5f, 0.0F), b2_pi);
    std::cout << "Right arm position: (" << m_RightArm->GetPosition().x << ", " << m_RightArm->GetPosition().y << ")\n";

    // Brazo izquierdo
    m_LeftArm = world->CreateBody(&bodyDef);
    m_LeftArm->SetTransform(position + b2Vec2(4.5f, 0.0F), b2_pi);
    std::cout << "Left arm position: (" << m_LeftArm->GetPosition().x << ", " << m_LeftArm->GetPosition().y << ")\n";

    // Pierna derecha
    m_RightLeg = world->CreateBody(&bodyDef);
    m_RightLeg->SetTransform(position + b2Vec2(1.5f, 10.0F), b2_pi);
    std::cout << "Right leg position: (" << m_RightLeg->GetPosition().x << ", " << m_RightLeg->GetPosition().y << ")\n";

    // Pierna izquierda
    m_LeftLeg = world->CreateBody(&bodyDef);
    m_LeftLeg->SetTransform(position + b2Vec2(1.5f, 10.0F), b2_pi);
    std::cout << "Left leg position: (" << m_LeftLeg->GetPosition().x << ", " << m_LeftLeg->GetPosition().y << ")\n";

    // Fixture
    b2PolygonShape bodyShape;
    b2PolygonShape limbShape;
    b2CircleShape headShape;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f; 
    fixtureDef.restitution = 0.2f;

    // Torso
    bodyShape.SetAsBox(3.0f, 5.0f);
    fixtureDef.shape = &bodyShape;
    m_Body->CreateFixture(&fixtureDef);

    // Cabeza
    headShape.m_radius = 2.0f;
    fixtureDef.shape = &headShape;
    m_Head->CreateFixture(&fixtureDef);

    // Brazos
    limbShape.SetAsBox(1.5f, 4.0f);
    fixtureDef.shape = &limbShape;
    m_RightArm->CreateFixture(&fixtureDef);
    m_LeftArm->CreateFixture(&fixtureDef);

    // Piernas
    limbShape.SetAsBox(1.5f, 5.0f);
    fixtureDef.shape = &limbShape;
    m_RightLeg->CreateFixture(&fixtureDef);
    m_LeftLeg->CreateFixture(&fixtureDef);

    // Joints 
    b2RevoluteJointDef jointDef;
    jointDef.lowerAngle = -0.4f * b2_pi;
    jointDef.upperAngle = 0.4f * b2_pi;
    jointDef.enableLimit = true;

    // Cuello
    jointDef.bodyA = m_Body;
    jointDef.bodyB = m_Head;
    jointDef.localAnchorA.Set(0.0f, 5.0f);
    jointDef.localAnchorB.Set(0.0f, -2.0f);
    m_NeckJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

    // Hombro derecho
    jointDef.bodyA = m_Body;
    jointDef.bodyB = m_RightArm;
    jointDef.localAnchorA.Set(3.0f, 4.0f);
    jointDef.localAnchorB.Set(0.0f, 4.0f);
    m_RightShoulderJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

    // Hombro izquierdo
    jointDef.bodyA = m_Body;
    jointDef.bodyB = m_LeftArm;
    jointDef.localAnchorA.Set(-3.0f, 4.0f);
    jointDef.localAnchorB.Set(0.0f, 4.0f);
    m_LeftShoulderJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

    // Cadera derecha
    jointDef.bodyA = m_Body;
    jointDef.bodyB = m_RightLeg;
    jointDef.localAnchorA.Set(1.5f, -5.0f);
    jointDef.localAnchorB.Set(0.0f, 5.0f);
    m_RightHipJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

    // Cadera izquierda
    jointDef.bodyA = m_Body;
    jointDef.bodyB = m_LeftLeg;
    jointDef.localAnchorA.Set(-1.5f, -5.0f);
    jointDef.localAnchorB.Set(0.0f, 5.0f);
    m_LeftHipJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

    // Formas
    m_BodyShape.setSize(sf::Vector2f(6.0f, 10.0f));
    m_HeadShape.setRadius(2.0f);
    m_RightArmShape.setSize(sf::Vector2f(3.0f, 8.0f));
    m_LeftArmShape.setSize(sf::Vector2f(3.0f, 8.0f));
    m_RightLegShape.setSize(sf::Vector2f(3.0f, 10.0f));
    m_LeftLegShape.setSize(sf::Vector2f(3.0f, 10.0f));

    // Seteo origins
    m_BodyShape.setOrigin(3.0f, 5.0f);
    m_HeadShape.setOrigin(2.0f, 2.0f);
    m_RightArmShape.setOrigin(1.5f, 4.0f);
    m_LeftArmShape.setOrigin(1.5f, 4.0f);
    m_RightLegShape.setOrigin(1.5f, 5.0f);
    m_LeftLegShape.setOrigin(1.5f, 5.0f);

    // Seteo Colores
    m_BodyShape.setFillColor(sf::Color::Blue);
    m_HeadShape.setFillColor(sf::Color::Red);
    m_RightArmShape.setFillColor(sf::Color::Green);
    m_LeftArmShape.setFillColor(sf::Color::Green);
    m_RightLegShape.setFillColor(sf::Color::Yellow);
    m_LeftLegShape.setFillColor(sf::Color::Yellow);
}

Ragdoll::~Ragdoll()
{
    // Box2D bodies and joints are destroyed when the world is destroyed
}

void Ragdoll::Actualizar()
{    
    m_BodyShape.setPosition((m_Body->GetPosition().x), (m_Body->GetPosition().y));
    m_BodyShape.setRotation(m_Body->GetAngle() * 180.0f / b2_pi);

    m_HeadShape.setPosition((m_Head->GetPosition().x), (m_Head->GetPosition().y));
    m_HeadShape.setRotation(m_Head->GetAngle() * 180.0f / b2_pi);

    m_RightArmShape.setPosition((m_RightArm->GetPosition().x), (m_RightArm->GetPosition().y));
    m_RightArmShape.setRotation(m_RightArm->GetAngle() * 180.0f / b2_pi);

    m_LeftArmShape.setPosition((m_LeftArm->GetPosition().x), (m_LeftArm->GetPosition().y));
    m_LeftArmShape.setRotation(m_LeftArm->GetAngle() * 180.0f / b2_pi);

    m_RightLegShape.setPosition((m_RightLeg->GetPosition().x), (m_RightLeg->GetPosition().y));
    m_RightLegShape.setRotation(m_RightLeg->GetAngle() * 180.0f / b2_pi);

    m_LeftLegShape.setPosition((m_LeftLeg->GetPosition().x), (m_LeftLeg->GetPosition().y));
    m_LeftLegShape.setRotation(m_LeftLeg->GetAngle() * 180.0f / b2_pi);
}

void Ragdoll::Dibujar(sf::RenderWindow& window)
{
    window.draw(m_BodyShape);
    window.draw(m_HeadShape);
    window.draw(m_RightArmShape);
    window.draw(m_LeftArmShape);
    window.draw(m_RightLegShape);
    window.draw(m_LeftLegShape);
}