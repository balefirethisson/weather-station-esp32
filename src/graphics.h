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
    
public:
    Graphics() {}
    
    void begin() {
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(COLOR_BG);
        tft.setTextColor(COLOR_WHITE, COLOR_BG);
    }
    
    void clearScreen() {
        tft.fillScreen(COLOR_BG);
    }
    
    void drawWiFiSetupScreen(String ssidText = "", String passwordText = "", int activeField = 0, bool webPortalActive = false) {
        clearScreen();
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_CYAN, COLOR_BG);
        tft.drawCentreString("WiFi Setup", 160, 15, 2);
        
        if (webPortalActive) {
            // Show web portal instructions
            tft.setTextSize(1);
            tft.setTextColor(COLOR_GREEN, COLOR_BG);
            tft.drawCentreString("Web Portal Active!", 160, 45, 2);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
            tft.drawCentreString("1. On your phone:", 160, 70, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawCentreString("Connect to WiFi:", 160, 85, 1);
            
            tft.setTextSize(2);
            tft.setTextColor(COLOR_ORANGE, COLOR_BG);
            tft.drawCentreString("WeatherStation-Setup", 160, 100, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawCentreString("(No password needed)", 160, 115, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
            tft.drawCentreString("2. Open browser:", 160, 135, 1);
            
            tft.setTextSize(2);
            tft.setTextColor(COLOR_ORANGE, COLOR_BG);
            tft.drawCentreString("192.168.4.1", 160, 150, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawCentreString("3. Select WiFi + Enter Password", 160, 170, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_GRAY, COLOR_BG);
            tft.drawCentreString("Portal timeout: 5 minutes", 160, 200, 1);
            tft.drawCentreString("Or use display below to setup manually", 160, 215, 1);
            
        } else {
            // Show display-based setup (manual entry)
            tft.setTextSize(1);
            tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
            tft.drawCentreString("(Display Setup - Phone Portal Failed)", 160, 40, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawString("Network (SSID):", 20, 70, 2);
            
            uint16_t boxColor = (activeField == 0) ? COLOR_LIGHT_BLUE : COLOR_GRAY;
            tft.drawRect(20, 90, 280, 30, boxColor);
            if (activeField == 0) {
                tft.fillRect(20, 90, 280, 30, COLOR_DARK_GRAY);
            }
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawString(ssidText.substring(0, 30), 25, 97, 2);
            
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawString("Password:", 20, 140, 2);
            
            boxColor = (activeField == 1) ? COLOR_LIGHT_BLUE : COLOR_GRAY;
            tft.drawRect(20, 160, 280, 30, boxColor);
            if (activeField == 1) {
                tft.fillRect(20, 160, 280, 30, COLOR_DARK_GRAY);
            }
            
            String dots = "";
            for (int i = 0; i < passwordText.length(); i++) {
                dots += "*";
            }
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawString(dots.substring(0, 30), 25, 167, 2);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_GRAY, COLOR_BG);
            tft.drawCentreString("Tap fields to edit | Swipe right to connect", 160, 215, 1);
        }
    }
    
    void drawLoadingScreen() {
        clearScreen();
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_CYAN, COLOR_BG);
        tft.drawCentreString("Connecting...", 160, 80, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
        tft.drawCentreString("Fetching weather data", 160, 110, 1);
        
        static int spinner = 0;
        String spinChar[] = {"|", "/", "-", "\\"};
        tft.drawString(spinChar[spinner % 4], 155, 140, 2);
        spinner++;
    }
    
    void drawCurrentWeather(String location, float temp, int humidity, float windSpeed,
                           String description, float feelsLike, int weatherId) {
        clearScreen();
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("1/4 CURRENT", 160, 5, 1);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_CYAN, COLOR_BG);
        tft.drawCentreString(location, 160, 20, 1);
        
        tft.setTextSize(6);
        tft.setTextColor(COLOR_ORANGE, COLOR_BG);
        String tempStr = String((int)temp) + "F";
        tft.drawString(tempStr, 30, 50, 6);
        
        WeatherIcons::drawWeatherIcon(tft, weatherId, 250, 80, 30, COLOR_LIGHT_BLUE);
        
        tft.setTextSize(2);
        tft.setTextColor(COLOR_WHITE, COLOR_BG);
        tft.drawCentreString(description, 160, 130, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
        tft.drawString("Feels: " + String((int)feelsLike) + "F", 20, 160, 2);
        tft.drawString("Humidity: " + String(humidity) + "%", 20, 180, 2);
        tft.drawString("Wind: " + String((int)windSpeed) + " mph", 20, 200, 2);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("< Swipe for hourly forecast >", 160, 225, 1);
    }
    
    void drawHourlyForecast(vector<HourlyData>& hours) {
        clearScreen();
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("2/4 HOURLY (24H)", 160, 5, 1);
        
        int cardWidth = 50;
        int spacing = 3;
        int startX = 10;
        
        for (int i = 0; i < hours.size() && i < 6; i++) {
            int x = startX + i * (cardWidth + spacing);
            
            tft.drawRect(x, 30, cardWidth, 190, COLOR_GRAY);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_CYAN, COLOR_BG);
            tft.drawString(hours[i].time, x + 3, 35, 1);
            
            WeatherIcons::drawWeatherIcon(tft, hours[i].weatherId, x + cardWidth/2, 65, 15, COLOR_LIGHT_BLUE);
            
            tft.setTextSize(2);
            tft.setTextColor(COLOR_ORANGE, COLOR_BG);
            tft.drawString(String((int)hours[i].temp) + "F", x + 2, 90, 2);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_BLUE, COLOR_BG);
            tft.drawString("R:" + String((int)hours[i].precipProb) + "%", x + 2, 110, 1);
            
            tft.setTextColor(COLOR_GREEN, COLOR_BG);
            tft.drawString("W:" + String((int)hours[i].windSpeed), x + 2, 125, 1);
        }
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("< Swipe for 5-day forecast >", 160, 225, 1);
    }
    
    void drawForecast5Day(vector<ForecastDay>& days) {
        clearScreen();
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("3/4 5-DAY FORECAST", 160, 5, 1);
        
        int yPos = 25;
        for (int i = 0; i < days.size() && i < 5; i++) {
            tft.setTextSize(1);
            tft.setTextColor(COLOR_CYAN, COLOR_BG);
            tft.drawString(days[i].date, 10, yPos, 1);
            
            WeatherIcons::drawWeatherIcon(tft, days[i].weatherId, 90, yPos + 10, 15, COLOR_LIGHT_BLUE);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_ORANGE, COLOR_BG);
            tft.drawString(String((int)days[i].tempMax) + "F", 120, yPos + 3, 1);
            
            tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
            tft.drawString(String((int)days[i].tempMin) + "F", 170, yPos + 3, 1);
            
            tft.setTextColor(COLOR_WHITE, COLOR_BG);
            tft.drawString(days[i].description, 215, yPos + 3, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_BLUE, COLOR_BG);
            tft.drawString("R:" + String((int)days[i].precipChance) + "%", 10, yPos + 18, 1);
            
            yPos += 40;
        }
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("< Swipe for 7-day & precipitation >", 160, 225, 1);
    }
    
    void drawForecast7DayWithPrecip(vector<ForecastDay>& days) {
        clearScreen();
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("4/4 7-DAY & PRECIPITATION", 160, 5, 1);
        
        float maxPrecip = 0.5;
        for (auto& day : days) {
            if (day.precipitation > maxPrecip) maxPrecip = day.precipitation;
        }
        if (maxPrecip == 0) maxPrecip = 1;
        
        int graphHeight = 60;
        int graphY = 100;
        int barWidth = 35;
        int spacing = 3;
        int graphX = 20;
        
        tft.drawLine(graphX, graphY, graphX, graphY - graphHeight, COLOR_GRAY);
        tft.drawLine(graphX, graphY, graphX + (barWidth + spacing) * 7, graphY, COLOR_GRAY);
        
        for (int i = 0; i < days.size() && i < 7; i++) {
            int barHeight = (days[i].precipitation / maxPrecip) * graphHeight;
            int x = graphX + i * (barWidth + spacing);
            int y = graphY - barHeight;
            
            uint16_t barColor;
            if (days[i].precipitation < 0.5) {
                barColor = COLOR_LIGHT_BLUE;
            } else if (days[i].precipitation < 1.0) {
                barColor = COLOR_BLUE;
            } else {
                barColor = COLOR_PURPLE;
            }
            
            tft.fillRect(x, y, barWidth, barHeight, barColor);
            tft.drawRect(x, y, barWidth, barHeight, COLOR_LIGHT_BLUE);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_GRAY, COLOR_BG);
            String dateShort = days[i].date.substring(5);
            tft.drawString(dateShort, x - 2, graphY + 5, 1);
            
            tft.setTextSize(1);
            tft.setTextColor(COLOR_CYAN, COLOR_BG);
            tft.drawString(String(days[i].precipitation, 1) + "\"", x - 5, y - 10, 1);
        }
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_LIGHT_BLUE, COLOR_BG);
        tft.drawString("Light:", 20, 180, 1);
        tft.fillRect(85, 178, 8, 8, COLOR_LIGHT_BLUE);
        
        tft.drawString("Moderate:", 140, 180, 1);
        tft.fillRect(210, 178, 8, 8, COLOR_BLUE);
        
        tft.drawString("Heavy:", 20, 200, 1);
        tft.fillRect(75, 198, 8, 8, COLOR_PURPLE);
        
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.drawCentreString("< Swipe to return to current >", 160, 225, 1);
    }
    
    void drawWiFiConnected(String ip) {
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GREEN, COLOR_BG);
        tft.drawString("Connected!", 20, 180, 1);
        tft.drawString("IP: " + ip, 20, 195, 1);
    }
    
    void drawWiFiError(String error) {
        tft.setTextSize(1);
        tft.setTextColor(COLOR_RED, COLOR_BG);
        tft.drawString("Error: " + error, 20, 180, 1);
    }
};

#endif
