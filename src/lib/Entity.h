
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Moveable {
  public:
    Moveable(b2Body* body);
    float getX();
    float getY();
    void setPos(float x, float y);

  private:
    b2Body* body_;
};

class Renderable {
  public:
    Renderable(sf::Shape* shape);
    void render(sf::RenderWindow* window);
    void setPos(float x, float y);

  private:
    sf::Shape* shape_;
};

class Entity {
  public:
    Entity(Moveable* moveable, Renderable* renderable);
    ~Entity();
    float getX();
    float getY();
    void setPos(float x, float y);
    void render(sf::RenderWindow* window);

  private:
    Moveable* moveable_;
    Renderable* renderable_;
};
