#ifndef TOUCH_HANDLER_H
#define TOUCH_HANDLER_H

#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define TOUCH_CS_PIN 33
#define TOUCH_IRQ 36

class TouchHandler {
private:
    XPT2046_Touchscreen ts;
    int lastTouchX = 0;
    int lastTouchY = 0;
    unsigned long lastTouchTime = 0;
    
public:
    enum SwipeDirection {
        NONE,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    
    TouchHandler() : ts(TOUCH_CS_PIN) {}
    
    void begin() {
        SPI.begin(18, 19, 23, 33);
        ts.begin();
        ts.setRotation(1);
    }
    
    bool isTouched() {
        return ts.touched();
    }
    
    TS_Point getTouchPoint() {
        if (ts.touched()) {
            TS_Point p = ts.getPoint();
            
            int x = map(p.x, 200, 3800, 0, 320);
            int y = map(p.y, 200, 3800, 0, 240);
            
            x = constrain(x, 0, 319);
            y = constrain(y, 0, 239);
            
            lastTouchX = x;
            lastTouchY = y;
            lastTouchTime = millis();
            
            return TS_Point(x, y, p.z);
        }
        return TS_Point(0, 0, 0);
    }
    
    int getLastX() { return lastTouchX; }
    int getLastY() { return lastTouchY; }
    unsigned long getLastTouchTime() { return lastTouchTime; }
};

#endif
