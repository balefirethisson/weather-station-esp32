#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <TFT_eSPI.h>
#include "config.h"
#include "weather_icons.h"
#include <vector>

using namespace std;

class Graphics {
private:
    TFT_eSPI tft;
    
    // Apple-inspired color palette
    const uint16_t COLOR_BG = 0x0000;           // Pure black background
    const uint16_t COLOR_CARD_BG = 0x1082;      // Dark blue-gray
    const uint16_t COLOR_TEXT_PRIMARY = 0xFFFF; // White
    const uint16_t COLOR_TEXT_SECONDARY = 0x8410; // Gray
    const uint16_t COLOR_BLUE = 0x049F;         // Apple blue
    const uint16_t COLOR_LIGHT_BLUE = 0x07FF;   // Cyan
    const uint16_t COLOR_ORANGE = 0xFD20;       // Orange
    const uint16_t COLOR_YELLOW = 0xFFE0;       // Yellow
    const uint16_t COLOR_PURPLE = 0x9819;       // Purple
    const uint16_t COLOR_RED = 0xF800;          // Red
    const uint16_t COLOR_GREEN = 0x07E0;        // Green
    const uint16_t COLOR_CYAN = 0x07FF;         // Cyan
    
public:
    Graphics() {}
    
    void begin() {
        tft.init();
        tft.setRotation(1);  // Landscape
        tft.fillScreen(COLOR_BG);
    }
    
    void clearScreen() {
        tft.fillScreen(COLOR_BG);
    }
    
    // ============================================
    // SCREEN 1: WiFi Setup - iPhone Style
    // ============================================
    void drawWiFiSetupScreen(String ssidText = "", String passwordText = "", int activeField = 0, bool webPortalActive = false) {
        clearScreen();
        
        if (webPortalActive) {
            drawWebPortalSetupiPhone();
        } else {
            drawDisplaySetupiPhone(ssidText, passwordText, activeField);
        }
    }
    
    void drawWebPortalSetupiPhone() {
        // Clean minimal header
        tft.setTextSize(3);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("WiFi", 160, 20, 2);
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString("Setup via Phone", 160, 50, 1);
        
        // Status pill
        int pillY = 85;
        tft.fillRoundRect(90, pillY, 140, 30, 15, COLOR_GREEN);
        tft.setTextSize(2);
        tft.setTextColor(COLOR_BG, COLOR_GREEN);
        tft.drawCentreString("Active", 160, pillY + 8, 2);
        
        // Main content - minimal style
        int contentY = 130;
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawString("Step 1", 20, contentY, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawString("Go to WiFi Settings", 20, contentY + 20, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_ORANGE, COLOR_BG);
        tft.drawString("WeatherStation-Setup", 20, contentY + 38, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawString("Step 2", 20, contentY + 62, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawString("Open Browser", 20, contentY + 80, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
        tft.drawString("192.168.4.1", 20, contentY + 98, 2);
    }
    
    void drawDisplaySetupiPhone(String ssidText, String passwordText, int activeField) {
        // Minimal header
        tft.setTextSize(3);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("WiFi", 160, 20, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString("Enter your network details", 160, 50, 1);
        
        // SSID Field
        int fieldY = 85;
        uint16_t ssidFieldColor = (activeField == 0) ? COLOR_LIGHT_BLUE : COLOR_TEXT_SECONDARY;
        
        // Underline style (iOS-like)
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawString("Network", 20, fieldY, 2);
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawString(ssidText.substring(0, 25), 20, fieldY + 20, 2);
        
        // Underline
        tft.drawLine(20, fieldY + 38, 300, fieldY + 38, ssidFieldColor);
        if (activeField == 0) {
            tft.drawLine(20, fieldY + 39, 300, fieldY + 39, ssidFieldColor);
        }
        
        // Password Field
        int passFieldY = fieldY + 70;
        uint16_t passFieldColor = (activeField == 1) ? COLOR_LIGHT_BLUE : COLOR_TEXT_SECONDARY;
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawString("Password", 20, passFieldY, 2);
        
        String dots = "";
        for (int i = 0; i < passwordText.length(); i++) {
            dots += "*";
        }
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawString(dots.substring(0, 20), 20, passFieldY + 20, 2);
        
        // Underline
        tft.drawLine(20, passFieldY + 38, 300, passFieldY + 38, passFieldColor);
        if (activeField == 1) {
            tft.drawLine(20, passFieldY + 39, 300, passFieldY + 39, passFieldColor);
        }
        
        // Action hint
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString("Swipe right to connect", 160, 220, 1);
    }
    
    // ============================================
    // SCREEN 2: Current Weather - iPhone Style
    // ============================================
    void drawCurrentWeather(String location, float temp, int humidity, float windSpeed,
                           String description, float feelsLike, int weatherId) {
        clearScreen();
        
        // Location at top - minimal
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString(location, 160, 15, 2);
        
        // Large temperature - center of screen
        tft.setTextSize(7);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString(String((int)temp), 160, 60, 7);
        
        tft.setTextSize(3);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawString("F", 250, 65, 3);
        
        // Weather icon next to temperature
        WeatherIcons::drawWeatherIcon(tft, weatherId, 110, 85, 40, COLOR_LIGHT_BLUE);
        
        // Description
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString(description, 160, 135, 2);
        
        // Feels like
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString("Feels like " + String((int)feelsLike) + "F", 160, 155, 1);
        
        // Details in cards below
        drawWeatherCardsIPhone(humidity, windSpeed);
        
        // Page indicator at bottom
        drawPageIndicatorIPhone(1, 4);
    }
    
    void drawWeatherCardsIPhone(int humidity, float windSpeed) {
        int cardY = 180;
        int cardWidth = 140;
        int cardHeight = 45;
        
        // Humidity Card
        tft.fillRoundRect(15, cardY, cardWidth, cardHeight, 8, COLOR_CARD_BG);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_CARD_BG);
        tft.drawString("Humidity", 25, cardY + 5, 1);
        
        tft.setTextSize(3);
        tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_CARD_BG);
        tft.drawString(String(humidity) + "%", 25, cardY + 18, 3);
        
        // Wind Card
        tft.fillRoundRect(165, cardY, cardWidth, cardHeight, 8, COLOR_CARD_BG);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_CARD_BG);
        tft.drawString("Wind", 175, cardY + 5, 1);
        
        tft.setTextSize(3);
        tft.setTextColor(COLOR_GREEN, COLOR_CARD_BG);
        tft.drawString(String((int)windSpeed) + "mph", 175, cardY + 18, 2);
    }
    
    // ============================================
    // SCREEN 3: Hourly Forecast - iPhone Style
    // ============================================
    void drawHourlyForecast(vector<HourlyData>& hours) {
        clearScreen();
        
        // Header
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("Hourly", 160, 15, 2);
        
        // Horizontal scrollable hourly cards
        int cardX = 5;
        int cardY = 50;
        int cardWidth = 50;
        int cardHeight = 165;
        
        for (int i = 0; i < hours.size() && i < 6; i++) {
            // Card background
            tft.fillRoundRect(cardX, cardY, cardWidth, cardHeight, 6, COLOR_CARD_BG);
            
            // Time
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_CARD_BG);
            tft.drawCentreString(hours[i].time, cardX + cardWidth/2, cardY + 5, 1);
            
            // Icon
            WeatherIcons::drawWeatherIcon(tft, hours[i].weatherId, cardX + cardWidth/2, cardY + 35, 14, COLOR_LIGHT_BLUE);
            
            // Temperature
            tft.setTextSize(2);
            tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_CARD_BG);
            tft.drawCentreString(String((int)hours[i].temp) + "F", cardX + cardWidth/2, cardY + 65, 2);
            
            // Rain chance with bar
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_CARD_BG);
            tft.drawCentreString("R:" + String((int)hours[i].precipProb) + "%", cardX + cardWidth/2, cardY + 85, 1);
            
            // Rain bar
            int barHeight = (hours[i].precipProb / 100.0) * 30;
            uint16_t barColor = (hours[i].precipProb < 30) ? COLOR_LIGHT_BLUE : 
                               (hours[i].precipProb < 60) ? COLOR_BLUE : COLOR_PURPLE;
            tft.fillRoundRect(cardX + 5, cardY + 115 - barHeight, cardWidth - 10, barHeight, 2, barColor);
            
            // Wind
            tft.setTextSize(1);
            tft.setTextColor(COLOR_GREEN, COLOR_CARD_BG);
            tft.drawCentreString(String((int)hours[i].windSpeed), cardX + cardWidth/2, cardY + 150, 1);
            
            cardX += cardWidth + 2;
        }
        
        // Page indicator
        drawPageIndicatorIPhone(2, 4);
    }
    
    // ============================================
    // SCREEN 4: 5-Day Forecast - iPhone Style
    // ============================================
    void drawForecast5Day(vector<ForecastDay>& days) {
        clearScreen();
        
        // Header
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("5-Day Forecast", 160, 15, 2);
        
        int itemY = 50;
        
        for (int i = 0; i < days.size() && i < 5; i++) {
            // Divider line
            if (i > 0) {
                tft.drawLine(20, itemY - 5, 300, itemY - 5, COLOR_TEXT_SECONDARY);
            }
            
            // Date
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
            tft.drawString(days[i].date, 20, itemY, 2);
            
            // Icon
            WeatherIcons::drawWeatherIcon(tft, days[i].weatherId, 100, itemY + 5, 16, COLOR_LIGHT_BLUE);
            
            // High / Low
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
            tft.drawString(String((int)days[i].tempMax) + "F", 200, itemY, 2);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
            tft.drawString(String((int)days[i].tempMin) + "F", 250, itemY, 2);
            
            // Description
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
            tft.drawString(days[i].description, 20, itemY + 18, 1);
            
            itemY += 35;
        }
        
        // Page indicator
        drawPageIndicatorIPhone(3, 4);
    }
    
    // ============================================
    // SCREEN 5: Precipitation Graph - iPhone Style
    // ============================================
    void drawForecast7DayWithPrecip(vector<ForecastDay>& days) {
        clearScreen();
        
        // Header
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("Precipitation", 160, 15, 2);
        
        // Graph container
        int graphY = 50;
        int graphHeight = 100;
        int graphX = 15;
        int graphWidth = 290;
        
        tft.drawRoundRect(graphX, graphY, graphWidth, graphHeight, 8, COLOR_TEXT_SECONDARY);
        
        // Find max precipitation
        float maxPrecip = 0.5;
        for (auto& day : days) {
            if (day.precipitation > maxPrecip) maxPrecip = day.precipitation;
        }
        if (maxPrecip == 0) maxPrecip = 1;
        
        // Draw bars
        int barWidth = 35;
        int spacing = 2;
        int barStartX = graphX + 10;
        int barBaseY = graphY + graphHeight - 10;
        
        for (int i = 0; i < days.size() && i < 7; i++) {
            int barHeight = (days[i].precipitation / maxPrecip) * (graphHeight - 20);
            int x = barStartX + i * (barWidth + spacing);
            int y = barBaseY - barHeight;
            
            // Bar color based on amount
            uint16_t barColor = (days[i].precipitation < 0.3) ? COLOR_LIGHT_BLUE : 
                               (days[i].precipitation < 0.7) ? COLOR_BLUE : 
                               (days[i].precipitation < 1.5) ? COLOR_PURPLE : COLOR_RED;
            
            tft.fillRoundRect(x, y, barWidth - 2, barHeight, 2, barColor);
            
            // Day label
            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
            String dayLabel = days[i].date.substring(5);
            tft.drawCentreString(dayLabel, x + barWidth/2, barBaseY + 5, 1);
        }
        
        // Legend below graph
        int legendY = graphY + graphHeight + 15;
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        
        // Light rain
        tft.fillRect(20, legendY, 8, 8, COLOR_LIGHT_BLUE);
        tft.drawString("< 0.3\"", 35, legendY - 1, 1);
        
        // Moderate rain
        tft.fillRect(110, legendY, 8, 8, COLOR_BLUE);
        tft.drawString("0.3-0.7\"", 125, legendY - 1, 1);
        
        // Heavy rain
        tft.fillRect(210, legendY, 8, 8, COLOR_RED);
        tft.drawString("> 1.5\"", 225, legendY - 1, 1);
        
        // Specific precipitation amounts
        int amountY = legendY + 25;
        tft.setTextSize(1);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        
        for (int i = 0; i < days.size() && i < 7; i++) {
            int x = barStartX + i * (barWidth + spacing);
            tft.drawCentreString(String(days[i].precipitation, 1) + "\"", x + barWidth/2, amountY, 1);
        }
        
        // Page indicator
        drawPageIndicatorIPhone(4, 4);
    }
    
    // ============================================
    // Loading Screen - iPhone Style
    // ============================================
    void drawLoadingScreen() {
        clearScreen();
        
        tft.setTextSize(3);
        tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BG);
        tft.drawCentreString("Loading", 160, 80, 3);
        
        // Animated dots
        static int frames = 0;
        String dots = "";
        for (int i = 0; i < (frames % 3) + 1; i++) {
            dots += ".";
        }
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_TEXT_SECONDARY, COLOR_BG);
        tft.drawCentreString(dots, 160, 120, 2);
        
        frames++;
    }
    
private:
    // Helper Functions
    
    void drawPageIndicatorIPhone(int current, int total) {
        int dotY = 225;
        int dotSpacing = 8;
        int totalWidth = (total - 1) * dotSpacing;
        int startX = 160 - totalWidth / 2;
        
        for (int i = 0; i < total; i++) {
            uint16_t color = (i == current - 1) ? COLOR_TEXT_PRIMARY : COLOR_TEXT_SECONDARY;
            int size = (i == current - 1) ? 4 : 3;
            tft.fillCircle(startX + i * dotSpacing, dotY, size, color);
        }
    }
};

#endif