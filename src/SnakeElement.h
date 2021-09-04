#ifndef __SNAKEELEMENT_H__
#define __SNAKEELEMENT_H__

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

enum SnakeDirection {stop, left, right, up, down};
extern Adafruit_SSD1306 display;

class Circle
{
  public:
    Circle(): X(0), Y(0), R(0) {};
    Circle(int x, int y, int r): X(x), Y(y), R(r) {};
    Circle(const Circle & cCircle);
    ~Circle(){};
    void setX(const int &x);
    void setY(const int &y);
    int getX();
    int getY();
    void getXYR(int &x, int &y, int &r);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void stop_moving();
    void move(int direction);
    void draw();
    bool intersects(Circle &other);
  private:
    int X, Y, R;

};

class SnakeElement
{
  public:
    SnakeElement();
    SnakeElement(int x, int y, int r);
    ~SnakeElement();
    bool update(int newDir);
    void draw();
    void setPosition(int x, int y);
    bool elongate();
    bool eating(Circle &Food);
    bool checkPosition(int x, int y);
    void detachTail();
    bool check_collision(SnakeElement *elem=nullptr);
    bool print_all_element();
  private:
    static int TailCount;
    int CurrentDirection;
    Circle Head;
    SnakeElement *mHead;
    SnakeElement *mTail;
};

#endif // __SNAKEELEMENT_H__