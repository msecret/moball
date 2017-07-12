
#include "Entity.h"

const float DRAW_FACTOR = 10.0f;


Moveable::Moveable(b2Body* body) :
  body_(body)
{}

float Moveable::getX()
{
  return body_->GetPosition().x;
}

float Moveable::getY()
{
  return body_->GetPosition().x;
}

void Moveable::setPos(float x, float y)
{
  return body_->SetTransform(b2Vec2(x, y), 0);
}

Renderable::Renderable(sf::Shape* shape) :
  shape_(shape)
{ }

void Renderable::render(sf::RenderWindow* window)
{
  window->draw(*shape_);
}

void Renderable::setPos(float x, float y)
{
  float tx = x * DRAW_FACTOR;
  float ty = y * DRAW_FACTOR;
  shape_->setPosition(tx, ty);
}


Entity::Entity(Moveable* moveable, Renderable* renderable) :
  renderable_(renderable),
  moveable_(moveable)
{}

Entity::~Entity()
{
  delete moveable_;
  delete renderable_;
}

float Entity::getX()
{
  return moveable_->getX();
}

float Entity::getY()
{
  return moveable_->getY();
}

void Entity::setPos(float x, float y)
{
  moveable_->setPos(x, y);
}

void Entity::render(sf::RenderWindow* window)
{
  float posX = getX();
  float posY = getY();
  renderable_->setPos(posX, posY);
  renderable_->render(window);
}
