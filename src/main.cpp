
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

const float DRAW_FACTOR = 10.0f;

b2Vec2 transformForDrawing(const b2Vec2& val)
{
  b2Vec2 transformed;
  transformed.x = val.x * DRAW_FACTOR;
  transformed.y = val.y * DRAW_FACTOR;
  return transformed;
}

int main()
{
  b2Vec2 gravity(0.0f, 0.0f);
  b2World world(gravity);
  world.SetContinuousPhysics(true);
  float32 timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  float pw = 5.0f;
  float ph = 5.0f;
  b2BodyDef playerDef;
  playerDef.type = b2_dynamicBody;
  playerDef.position.Set(10.0f, 10.0f);
  b2Body* player = world.CreateBody(&playerDef);
  b2PolygonShape playerBox;
  playerBox.SetAsBox(pw, ph);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &playerBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  player->CreateFixture(&fixtureDef);
  b2Vec2 force(5000.0f, 1000.0f);
  player->ApplyForceToCenter(force, true);

  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

  sf::RectangleShape playerR;
  playerR.setSize(sf::Vector2f(pw * DRAW_FACTOR, ph * DRAW_FACTOR));
  playerR.setFillColor(sf::Color::Green);
  b2Vec2 pPos = player->GetPosition();
  playerR.setPosition(pPos.x, pPos.y);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 pPos = player->GetPosition();
    float32 pAngle = player->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", pPos.x, pPos.y, pAngle);

    b2Vec2 transformedPos = transformForDrawing(pPos);
    playerR.setPosition(transformedPos.x, transformedPos.y);
    playerR.setRotation(pAngle);

    window.clear();
    window.draw(playerR);
    window.display();
  }

  return 0;
}

