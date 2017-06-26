
#include "Entity.h"

Moveable::Moveable() :
  x_(0),
  y_(0)
{}

Moveable::Moveable(float x, float y) :
  x_(x),
  y_(y)
{}

float Moveable::getX()
{
  return x_;
}

float Moveable::getY()
{
  return y_;
}

void Moveable::move(float x, float y)
{
  x_ = x;
  y_ = y;
}

Entity::Entity() :
  moveable_(new Moveable)
{}

Entity::~Entity()
{
  delete moveable_;
}

Moveable* Entity::getMoveable()
{
  return moveable_;
}
