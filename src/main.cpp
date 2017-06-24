#include <vector>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

const float DRAW_FACTOR = 10.0f;
const float width = 60.0f;
const float height = 36.0f;
const float PLAYER_R = 0.35;
const float BALL_R = 0.23;

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
  b2Vec2 walls[16];
  // Extra padding added to walls so they're full on screen
  const float w = 40.0f;
  const float h = 24.0f;
  walls[0].Set(-width * .5 +0.2 +w, -height * .5 +h);
  walls[1].Set(-width * .5 +w, -height * .5 +0.2 +h);
  walls[2].Set(-width * .5 +w, -height * .2 +h);
  walls[3].Set(-width * .6 +w, -height * .2 +h);
  walls[4].Set(-width * .6 +w, +height * .2 +h);
  walls[5].Set(-width * .5 +w, +height * .2 +h);
  walls[6].Set(-width * .5 +w, +height * .5 -.2 +h);
  walls[7].Set(-width * .5 +.2 +w, +height * .5 +h);
  walls[8].Set(+width * .5 -.2 +w, +height * .5 +h);
  walls[9].Set(+width * .5 +w, +height * .5 -.2 +h);
  walls[10].Set(+width * .5 +w, +height * .2 +h);
  walls[11].Set(+width * .6 +w, +height * .2 +h);
  walls[12].Set(+width * .6 +w, -height * .2 +h);
  walls[13].Set(+width * .5 +w, -height * .2 +h);
  walls[14].Set(+width * .5 +w, -height * .5 +.2 +h);
  walls[15].Set(+width * .5 -.2 +w, -height * .5 +h);

  b2ChainShape wallChain;
  wallChain.CreateChain(walls, 16);

  b2FixtureDef wallFixDef;
  wallFixDef.shape = &wallChain;
  wallFixDef.friction = 0;
  wallFixDef.restitution = 0;

  b2BodyDef wallDef;
  wallDef.type = b2_dynamicBody;
  b2Body* wall = world.CreateBody(&wallDef);
  wall->CreateFixture(&wallFixDef);

  b2BodyDef playerDef;
  playerDef.type = b2_dynamicBody;
  playerDef.position.Set(50.0f, 10.0f);
  b2Body* player = world.CreateBody(&playerDef);
  b2PolygonShape playerBox;
  playerBox.SetAsBox(pw, ph);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &playerBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  player->CreateFixture(&fixtureDef);
  b2Vec2 force(-5000.0f, 1000.0f);
  player->ApplyForceToCenter(force, true);

  sf::RenderWindow window(sf::VideoMode(1250, 800), "SFML works!");

  sf::VertexArray wallLines(sf::LinesStrip, 16);
  for( unsigned int i = 0; i < sizeof(walls)/sizeof(walls[0]); i = i + 1 )
  {
    b2Vec2 transformed = transformForDrawing(walls[i]);
    wallLines[i].position = sf::Vector2f(transformed.x, transformed.y);
    wallLines[i].color = sf::Color::Red;
    printf("%4.2f %4.2f \n", transformed.x, transformed.y);
  }

  printf("%4.2f %4.2f %4.2f %4.2f \n", wallLines.getBounds().left, wallLines.getBounds().top, wallLines.getBounds().width, wallLines.getBounds().height);

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
    //printf("%4.2f %4.2f %4.2f\n", pPos.x, pPos.y, pAngle);

    b2Vec2 transformedPos = transformForDrawing(pPos);
    playerR.setPosition(transformedPos.x, transformedPos.y);
    playerR.setRotation(pAngle);

    window.clear();
    window.draw(playerR);
    window.draw(wallLines);
    window.display();
  }

  return 0;
}

