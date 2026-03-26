#ifndef WEATHER_ICONS_H
#define WEATHER_ICONS_H

#include <TFT_eSPI.h>

class WeatherIcons {
public:
    // Draw weather icon based on OpenWeatherMap ID
    static void drawWeatherIcon(TFT_eSPI& tft, int weatherId, int x, int y, int size, uint16_t color) {
        if (weatherId >= 200 && weatherId <= 232) {
            drawThunderstorm(tft, x, y, size, color);
        } 
        else if (weatherId >= 300 && weatherId <= 321) {
            drawDrizzle(tft, x, y, size, color);
        } 
        else if (weatherId >= 500 && weatherId <= 531) {
            drawRain(tft, x, y, size, color);
        } 
        else if (weatherId >= 600 && weatherId <= 622) {
            drawSnow(tft, x, y, size, color);
        } 
        else if (weatherId >= 701 && weatherId <= 781) {
            drawMist(tft, x, y, size, color);
        } 
        else if (weatherId == 800) {
            drawSunny(tft, x, y, size, color);
        } 
        else if (weatherId >= 801 && weatherId <= 804) {
            if (weatherId == 801) drawFewClouds(tft, x, y, size, color);
            else if (weatherId == 802) drawScatteredClouds(tft, x, y, size, color);
            else drawBrokenClouds(tft, x, y, size, color);
        }
    }

private:
    // ☀️ SUNNY (ID: 800)
    static void drawSunny(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        int radius = size / 2;
        
        // Sun circle
        tft.fillCircle(cx, cy, radius - 2, TFT_YELLOW);
        tft.drawCircle(cx, cy, radius - 2, color);
        
        // Rays
        int rayLength = radius + 3;
        int rayThickness = 2;
        
        // Top ray
        tft.fillRect(cx - rayThickness/2, cy - rayLength, rayThickness, 5, TFT_YELLOW);
        // Bottom ray
        tft.fillRect(cx - rayThickness/2, cy + rayLength - 5, rayThickness, 5, TFT_YELLOW);
        // Left ray
        tft.fillRect(cx - rayLength, cy - rayThickness/2, 5, rayThickness, TFT_YELLOW);
        // Right ray
        tft.fillRect(cx + rayLength - 5, cy - rayThickness/2, 5, rayThickness, TFT_YELLOW);
        
        // Diagonal rays
        tft.drawLine(cx - 8, cy - 8, cx - 12, cy - 12, TFT_YELLOW);
        tft.drawLine(cx + 8, cy - 8, cx + 12, cy - 12, TFT_YELLOW);
        tft.drawLine(cx - 8, cy + 8, cx - 12, cy + 12, TFT_YELLOW);
        tft.drawLine(cx + 8, cy + 8, cx + 12, cy + 12, TFT_YELLOW);
    }

    // ⛅ FEW CLOUDS (ID: 801)
    static void drawFewClouds(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Sun in background (small)
        int sunX = cx - 5;
        int sunY = cy - 5;
        tft.fillCircle(sunX, sunY, 4, TFT_YELLOW);
        tft.drawCircle(sunX, sunY, 4, color);
        
        // Cloud in foreground
        int cloudX = cx + 2;
        int cloudY = cy + 3;
        drawCloud(tft, cloudX, cloudY, size - 2, color);
    }

    // ☁️ SCATTERED CLOUDS (ID: 802)
    static void drawScatteredClouds(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Two overlapping clouds
        drawCloud(tft, cx - 3, cy, size - 2, color);
        drawCloud(tft, cx + 5, cy + 4, size - 3, color);
    }

    // ☁️ BROKEN CLOUDS (ID: 803, 804)
    static void drawBrokenClouds(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Three overlapping clouds (denser)
        drawCloud(tft, cx - 6, cy - 2, size - 1, color);
        drawCloud(tft, cx, cy + 1, size, color);
        drawCloud(tft, cx + 6, cy - 1, size - 2, color);
    }

    // 🌧️ RAIN (ID: 500-531)
    static void drawRain(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Cloud
        drawCloud(tft, cx, cy - 5, size - 2, color);
        
        // Rain drops
        uint16_t rainColor = TFT_BLUE;
        int dropX = cx - 8;
        int dropY = cy + 5;
        
        for (int i = 0; i < 3; i++) {
            drawRaindrop(tft, dropX + i * 8, dropY, 2, rainColor);
            drawRaindrop(tft, dropX + i * 8 + 4, dropY + 4, 2, rainColor);
        }
    }

    // 🌧️ DRIZZLE (ID: 300-321)
    static void drawDrizzle(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Cloud
        drawCloud(tft, cx, cy - 5, size - 2, color);
        
        // Light rain drops (smaller)
        uint16_t rainColor = TFT_CYAN;
        int dropX = cx - 6;
        int dropY = cy + 5;
        
        for (int i = 0; i < 3; i++) {
            drawRaindrop(tft, dropX + i * 6, dropY, 1, rainColor);
        }
    }

    // ⛈️ THUNDERSTORM (ID: 200-232)
    static void drawThunderstorm(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Dark cloud
        drawCloud(tft, cx, cy - 5, size, TFT_DARKGREY);
        
        // Heavy rain
        uint16_t rainColor = TFT_BLUE;
        for (int i = 0; i < 2; i++) {
            int dropX = cx - 8 + i * 8;
            int dropY = cy + 5;
            drawRaindrop(tft, dropX, dropY, 2, rainColor);
            drawRaindrop(tft, dropX + 4, dropY + 3, 2, rainColor);
        }
        
        // Lightning bolt
        drawLightning(tft, cx + 10, cy + 2, rainColor);
    }

    // ❄️ SNOW (ID: 600-622)
    static void drawSnow(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Cloud
        drawCloud(tft, cx, cy - 5, size - 2, color);
        
        // Snowflakes
        uint16_t snowColor = TFT_WHITE;
        int flakeX = cx - 8;
        int flakeY = cy + 5;
        
        for (int i = 0; i < 3; i++) {
            drawSnowflake(tft, flakeX + i * 8, flakeY, 2, snowColor);
            drawSnowflake(tft, flakeX + i * 8 + 4, flakeY + 4, 2, snowColor);
        }
    }

    // 🌫️ MIST/FOG (ID: 701-781)
    static void drawMist(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        // Horizontal lines representing fog
        uint16_t fogColor = TFT_LIGHTGREY;
        int lineY = cy - 6;
        
        for (int i = 0; i < 4; i++) {
            int lineLength = size + 2;
            int startX = cx - lineLength / 2;
            tft.drawLine(startX, lineY + i * 4, startX + lineLength, lineY + i * 4, fogColor);
            
            // Shorter lines for effect
            if (i % 2 == 0) {
                tft.drawLine(startX + 2, lineY + i * 4 + 2, startX + lineLength - 2, lineY + i * 4 + 2, fogColor);
            }
        }
    }

    // Helper: Draw cloud shape
    static void drawCloud(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        uint16_t cloudColor = TFT_LIGHTGREY;
        
        // Cloud body made of circles
        int radius = size / 3;
        tft.fillCircle(cx - radius, cy, radius, cloudColor);
        tft.fillCircle(cx, cy - radius + 2, radius, cloudColor);
        tft.fillCircle(cx + radius, cy, radius, cloudColor);
        tft.fillRect(cx - radius, cy, size, radius, cloudColor);
        
        // Outline
        tft.drawCircle(cx - radius, cy, radius, color);
        tft.drawCircle(cx, cy - radius + 2, radius, color);
        tft.drawCircle(cx + radius, cy, radius, color);
    }

    // Helper: Draw raindrop
    static void drawRaindrop(TFT_eSPI& tft, int x, int y, int size, uint16_t color) {
        // Teardrop shape
        tft.fillCircle(x, y, size, color);
        tft.fillTriangle(x - size, y, x + size, y, x, y + size + 2, color);
    }

    // Helper: Draw lightning bolt
    static void drawLightning(TFT_eSPI& tft, int x, int y, uint16_t color) {
        // Zigzag lightning pattern
        tft.drawLine(x, y, x + 2, y + 4, color);
        tft.drawLine(x + 2, y + 4, x - 1, y + 7, color);
        tft.drawLine(x - 1, y + 7, x + 1, y + 11, color);
        
        // Thicker lines
        tft.drawLine(x + 1, y, x + 3, y + 4, color);
        tft.drawLine(x + 3, y + 4, x, y + 7, color);
        tft.drawLine(x, y + 7, x + 2, y + 11, color);
    }

    // Helper: Draw snowflake
    static void drawSnowflake(TFT_eSPI& tft, int cx, int cy, int size, uint16_t color) {
        int s = size;
        // Six-pointed star pattern
        tft.drawLine(cx, cy - s, cx, cy + s, color);
        tft.drawLine(cx - s, cy, cx + s, cy, color);
        tft.drawLine(cx - s, cy - s, cx + s, cy + s, color);
        tft.drawLine(cx + s, cy - s, cx - s, cy + s, color);
    }
};

#endif