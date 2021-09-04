#include "SnakeElement.h"

void Circle::setX(const int &x) {
  X = x;
}

void Circle::setY(const int &y) {
  Y = y;
}

int Circle::getX() 
{
    return X;
}

int Circle::getY() 
{
    return Y;
}

void Circle::getXYR(int &x, int &y, int &r) {
  x = X;
  y = Y;
  r = R;
}

void Circle::move_up() {
  if (Y < 0) {
    Y = 60;
  }
  Y -= 2 * R;
}
void Circle::move_down() {
  if (Y > 60) {
    Y = 0;
  }
  Y += 2 * R;
}
void Circle::move_left() {
  if (X < 0) {
    X = 124;
  }
  X -= 2 * R;
}
void Circle::move_right() {
  if (X > 124) {
    X = 0;
  }
  X += 2 * R;
}

void Circle::stop_moving() 
{
    X = X;
    Y = Y;
}


void Circle::draw() {
  display.fillCircle(X, Y, R, WHITE);
  //display.drawCircle(X, Y, R, BLACK);
}

bool Circle::intersects(Circle &other) {
  int xOther, yOther, rOther;
  other.getXYR(xOther, yOther, rOther);
  int distance = (((X - xOther) * (X - xOther)) + ((Y - yOther) * (Y - yOther)));
  return distance < ((R + rOther) * (R + rOther));
}



int SnakeElement::TailCount = 0;

SnakeElement::SnakeElement(): Head() {
  CurrentDirection = stop;
  mTail = NULL;
  mHead = NULL;
}

SnakeElement::SnakeElement(int x, int y, int r): Head(x, y, r) {
  CurrentDirection = stop;
  mTail = NULL;
  mHead = NULL;
}

SnakeElement::~SnakeElement() {
  Serial.println("Deleting!!!");
  if (NULL != mTail) {
    delete mTail;
  }
}

bool SnakeElement::update(int newDir) {
    int old_posX, old_posY, radius;
    Head.getXYR(old_posX, old_posY, radius);
    if ((NULL != mTail) && (!mTail->checkPosition(old_posX, old_posY))) {
        mTail->update(CurrentDirection);
    }
    switch (newDir) {
    case left:
        Head.move_left();
        break;
    case right:
        Head.move_right();
        break;
    case up:
        Head.move_up();
        break;
    case down:
        Head.move_down();
        break;
    case stop:
        break;
    }
    CurrentDirection = newDir;
    return true;

}

void SnakeElement::draw() {
  Head.draw();
  if (NULL != mTail) {
    mTail->draw();
  }
}

void SnakeElement::setPosition(int x, int y) 
{
    Head.setX(x);
    Head.setY(y);
}

bool SnakeElement::elongate() {
  if (TailCount >= 30) {
    return false;
  }
  if (NULL == mTail) { // New tail direct with head
    SnakeElement *newTail = NULL;
    int x, y, r;
    Head.getXYR(x, y, r);
    switch (CurrentDirection) {
      case left:
        newTail = new SnakeElement(x + 2 * r, y, r);
        break;
      case right:
        newTail = new SnakeElement(x - 2 * r, y, r);
        break;
      case up:
        newTail = new SnakeElement(x, y + 2 * r, r);
        break;
      case down:
        newTail = new SnakeElement(x, y - 2 * r, r);
        break;
    }
    newTail->mHead = this;
    mTail = newTail;
    TailCount++;
    return true;
  } else {
    return mTail->elongate();
  }
}

bool SnakeElement::eating(Circle &Food) {
  return Head.intersects(Food);
}

bool SnakeElement::checkPosition(int x, int y) 
{
    int posX, posY, radius;
    Head.getXYR(posX, posY, radius);
    return (x == posX) && (y == posY);
}

void SnakeElement::detachTail() 
{   
    SnakeElement *t = this->mTail;
    delete (t);
    mTail = NULL;
    TailCount = 0;
}

bool SnakeElement::check_collision(SnakeElement *elem)
{    
    SnakeElement *temp;
    temp = mTail->mTail;
    while (temp != NULL)
    {
        if(Head.intersects(temp->Head)){
            Serial.println("Intersect!");
            return true;
        }
        temp = temp->mTail;
    }
    
    return false;
}

bool SnakeElement::print_all_element() 
{
    int cnt = 0;
    Serial.print("n: ");
    Serial.print(cnt);
    Serial.print("x: ");
    Serial.print(Head.getX());
    Serial.print("y: ");
    Serial.print(Head.getY());
    Serial.println();
    SnakeElement *temp;
    temp = mTail;

    while (temp != NULL)
    {
        cnt++;
        Serial.print("n: ");
        Serial.print(cnt);
        Serial.print("x: ");
        Serial.print(temp->Head.getX());
        Serial.print("y: ");
        Serial.print(temp->Head.getY());
        Serial.println();
        temp = temp->mTail;
    }
    Serial.println(" ---------- ");
    return true;
}

