#ifndef SLIDER_H
#define SLIDER_H

#include "Control.h"

class Slider: public Control
{
public:
    Slider(string label, float min, float max,
            int positionX, int positionY, int width, int height);
    void setValue(float *value);

    virtual bool updateControl(MouseState &state);
    virtual void drawControl();
    virtual string getType();

protected:
    float defaultValue;

    float min;
    float max;
    float *current;

    string label;

    bool dragging;

};

#endif

