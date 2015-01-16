#ifndef BUTTON_H
#define BUTTON_H

#include "Control.h"

class Button: public Control
{
public:
    Button(string label, int positionX, int positionY, int width, int height);
    virtual bool updateControl(MouseState &state);
    virtual void drawControl();
    virtual string getType();

    string getLabel();

protected:
    bool down;

    string label;

};

#endif
