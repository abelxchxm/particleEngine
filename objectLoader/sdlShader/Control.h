#ifndef CONTROL_H
#define CONTROL_H

#include "GLEngine.h"

#include <list>
#include <string>

using std::list;
using std::string;

struct MouseState
{
    int LeftButtonDown;
    int RightButtonDown;
    int MiddleButtonDown;

    int x;
    int y;

    MouseState()
    {
        LeftButtonDown = 0;
        RightButtonDown = 0;
        MiddleButtonDown = 0;

        x = 0;
        y = 0;
    }
};

class Control
{
public:
    Control(int positionX, int positionY, int width, int height);
    virtual ~Control();

    virtual bool updateControl(MouseState &state);
    virtual void drawControl() = 0;
    virtual string getType() = 0;

    void setPosition(int x, int y);
    void setSize(int width, int height);

    int getWidth();
    int getHeight();

public:
    static list<Control *> controls;

protected:
    bool inside;

    int posX, posY;
    int width, height;
};

Control *addControl(Control *control);

#endif
