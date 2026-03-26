#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <TFT_eSPI.h>

class WeatherIcons {
public:
    static void drawClearSky(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        tft.fillCircle(x, y, size/3, color);
        tft.drawLine(x, y-size/2, x, y-size, color);
        tft.drawLine(x, y+size/2, x, y+size, color);
        tft.drawLine(x-size/2, y, x-size, y, color);
        tft.drawLine(x+size/2, y, x+size, y, color);
    }
    
    static void drawCloudy(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        tft.fillCircle(x-size/4, y, size/3, color);
        tft.fillCircle(x+size/4, y, size/3, color);
        tft.fillRect(x-size/2, y-size/4, size, size/2, color);
    }
    
    static void drawRainy(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        tft.fillCircle(x-size/4, y-size/4, size/3, color);
        tft.fillCircle(x+size/4, y-size/4, size/3, color);
        tft.fillRect(x-size/2, y-size/6, size, size/3, color);
        uint16_t rainColor = 0x5FFF;
        tft.drawLine(x-size/3, y+size/3, x-size/3-size/6, y+size/2, rainColor);
        tft.drawLine(x, y+size/3, x-size/6, y+size/2, rainColor);
        tft.drawLine(x+size/3, y+size/3, x+size/6, y+size/2, rainColor);
    }
    
    static void drawSnowy(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        tft.fillCircle(x-size/4, y-size/4, size/3, color);
        tft.fillCircle(x+size/4, y-size/4, size/3, color);
        tft.fillRect(x-size/2, y-size/6, size, size/3, color);
        uint16_t snowColor = 0xFFFF;
        for (int i = 0; i < 3; i++) {
            int snowX = x - size/3 + i*size/3;
            int snowY = y + size/3;
            tft.drawLine(snowX-size/8, snowY, snowX+size/8, snowY, snowColor);
            tft.drawLine(snowX, snowY-size/8, snowX, snowY+size/8, snowColor);
        }
    }
    
    static void drawThunderstorm(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        tft.fillCircle(x-size/3, y-size/4, size/2, color);
        tft.fillCircle(x+size/3, y-size/4, size/2, color);
        tft.fillRect(x-size/2, y-size/6, size, size/2, color);
        uint16_t lightColor = 0xFFE0;
        tft.drawLine(x, y+size/4, x+size/6, y+size/2, lightColor);
        tft.drawLine(x+size/6, y+size/2, x-size/6, y+size/3, lightColor);
        tft.drawLine(x-size/6, y+size/3, x+size/4, y+size, lightColor);
    }
    
    static void drawFoggy(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        for (int i = 0; i < 3; i++) {
            tft.drawLine(x-size/2, y-size/4+i*size/4, x+size/2, y-size/4+i*size/4, color);
        }
    }
    
    static void drawWeatherIcon(TFT_eSPI& tft, int weatherId, int x, int y, int size, uint16_t color) {
        if (weatherId >= 200 && weatherId < 300) {
            drawThunderstorm(tft, x, y, size, color);
        } else if (weatherId >= 300 && weatherId < 400) {
            drawRainy(tft, x, y, size, color);
        } else if (weatherId >= 500 && weatherId < 600) {
            drawRainy(tft, x, y, size, color);
        } else if (weatherId >= 600 && weatherId < 700) {
            drawSnowy(tft, x, y, size, color);
        } else if (weatherId >= 700 && weatherId < 800) {
            drawFoggy(tft, x, y, size, color);
        } else if (weatherId == 800) {
            drawClearSky(tft, x, y, size, color);
        } else if (weatherId == 801 || weatherId == 802) {
            drawCloudy(tft, x, y, size, color);
        } else if (weatherId == 803 || weatherId == 804) {
            drawCloudy(tft, x, y, size, color);
        }
    }
};

#endif
