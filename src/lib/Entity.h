
class Moveable {
  public:
    Moveable();
    Moveable(float x, float y);
    float getX();
    float getY();
    void move(float x, float y);

  private:
    float x_;
    float y_;
};

class Entity {
  public:
    Entity();
    ~Entity();
    Moveable* getMoveable();

  private:
    Moveable* moveable_;
};
