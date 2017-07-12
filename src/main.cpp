#include <vector>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "lib/Entity.h"

const float DRAW_FACTOR = 10.0f;
const float width = 60.0f;
const float height = 36.0f;
const float BALL_R = 1.38f;
const float PLAYER_R = 2.1f;

/* TODO
 * - Add a data structure to keep track of entity data rather then pulling from
 *   both box2d and sfml.
 * - Create some sort of data structure for the vectors and allow to transform
 *   from box2d and sfml so don't have to think which is which.
 * - Figure out positioning differences with example (from center vs from edge)
 *   and fix.
 * - Data structure to keep entities, update and render them.
 */

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

  std::vector<Entity*> entities;

  // TODO move to separate function or keep as separate data structure.
  // TODO fix top wall not appearing.
  // TODO figure out why extra padding needed and remove need for it (maybe by
  // setting position from the center somehow.
  // Wall creation
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
  b2Body* wall = world.CreateBody(&wallDef);
  wall->CreateFixture(&wallFixDef);


  // TODO move to new function
  // TODO remove beginning force.
  // TODO center ball
  // Ball creation
  b2CircleShape ballCircle;
  ballCircle.m_radius = BALL_R;
  b2FixtureDef ballFixDef;
  ballFixDef.shape = &ballCircle;
  ballFixDef.density = 0.2f;
  ballFixDef.friction = 0.5f;
  ballFixDef.restitution = 0.99f;
  b2BodyDef ballDef;
  ballDef.bullet = true;
  ballDef.linearDamping = 0.35f;
  ballDef.angularDamping = 0.6f;
  ballDef.type = b2_dynamicBody;
  ballDef.position.Set(20.0f, 20.0f);
  b2Body* ballBody = world.CreateBody(&ballDef);
  ballBody->CreateFixture(&ballFixDef);
  // Render ball
  sf::CircleShape ballR;
  ballR.setRadius(BALL_R * DRAW_FACTOR);
  ballR.setFillColor(sf::Color::White);
  Entity ball = Entity(
    new Moveable(ballBody),
    new Renderable(&ballR)
  );


  // TODO move to separate function.
  // TODO fix positioning so correct.
  // Player creation
  b2CircleShape playerCircle;
  playerCircle.m_radius = PLAYER_R;
  b2FixtureDef playerFixDef;
  playerFixDef.friction = 0.1f;
  playerFixDef.restitution = 0.99f;
  playerFixDef.density = 0.8f;
  playerFixDef.shape = &playerCircle;
  b2BodyDef playerDef;
  playerDef.type = b2_dynamicBody;
  playerDef.bullet = true;
  playerDef.linearDamping = 0.4f;
  playerDef.angularDamping = 0.16f;
  b2Vec2 playerVecs[5];
  playerVecs[0].Set(-width * .45 +w, 0 +h);
  playerVecs[1].Set(-width * .3 +w, -height * 0.2 +h);
  playerVecs[2].Set(-width * .3 +w, +height * 0.2 +h);
  playerVecs[3].Set(-width * .1 +w, -height * 0.1 +h);
  playerVecs[4].Set(-width * .1 +w, +height * 0.1 +h);
  b2Body* playerBodies[5];
  for(unsigned int i = 0; i < sizeof(playerVecs)/sizeof(playerVecs[0]); i = i + 1)
  {
    // TODO how to remove this and use in entity?
    playerDef.position.Set(playerVecs[i].x, playerVecs[i].y);
    b2Body* playerBody = world.CreateBody(&playerDef);
    playerBody->CreateFixture(&playerFixDef);
    playerBodies[i] = playerBody;
    playerDef.position.Set(playerVecs[i].x, playerVecs[i].y);
    sf::CircleShape playerR;
    playerR.setRadius(PLAYER_R * DRAW_FACTOR);
    playerR.setFillColor(sf::Color::Blue);
    Entity player = Entity(
      new Moveable(playerBody),
      new Renderable(&playerR)
    );
    playerBody->SetUserData(&player);
  }


  sf::RenderWindow window(sf::VideoMode(1250, 800), "SFML works!");


  // Render walls
  sf::VertexArray wallLines(sf::LinesStrip, 16);
  for( unsigned int i = 0; i < sizeof(walls)/sizeof(walls[0]); i = i + 1 )
  {
    b2Vec2 transformed = transformForDrawing(walls[i]);
    wallLines[i].position = sf::Vector2f(transformed.x, transformed.y);
    wallLines[i].color = sf::Color::Red;
  }
  printf("wall bounds: %4.2f %4.2f %4.2f %4.2f \n", wallLines.getBounds().left, wallLines.getBounds().top, wallLines.getBounds().width, wallLines.getBounds().height);


  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Body* bodies = world.GetBodyList();

    window.clear();
    while (bodies->GetNext() != NULL) {
      void* bodyUserData= bodies->GetUserData();
      if ( bodyUserData ) {
        Entity* entity = static_cast<Entity*>(bodyUserData);
        entity->render(&window);
      }
    }
    window.draw(wallLines);

    window.display();
  }

  return 0;
}

