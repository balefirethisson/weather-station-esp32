#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "graphics.h"
#include "touch_handler.h"
#include "config.h"
#include <vector>

using namespace std;

class UIManager {
private:
    Graphics graphics;
    TouchHandler touch;
    UIState currentState;
    UIState previousState;
    String currentSSID = "";
    String currentPassword = "";
    int activeInputField = 0;
    
public:
    UIManager() : currentState(WIFI_SETUP), previousState(WIFI_SETUP) {}
    
    void begin() {
        graphics.begin();
        touch.begin();
        currentState = WIFI_SETUP;
        drawCurrentState();
    }
    
    void drawCurrentState() {
        if (currentState == WIFI_SETUP) {
            graphics.drawWiFiSetupScreen(currentSSID, currentPassword, activeInputField);
        } else if (currentState == LOADING) {
            graphics.drawLoadingScreen();
        }
    }
    
    void setState(UIState newState) {
        previousState = currentState;
        currentState = newState;
    }
    
    UIState getState() { return currentState; }
    String getSSID() { return currentSSID; }
    String getPassword() { return currentPassword; }
    
    void setActiveField(int field) {
        activeInputField = field;
    }
    
    void addCharToActiveField(char c) {
        if (activeInputField == 0) {
            if (currentSSID.length() < 32) {
                currentSSID += c;
            }
        } else if (activeInputField == 1) {
            if (currentPassword.length() < 64) {
                currentPassword += c;
            }
        }
        drawCurrentState();
    }
    
    void removeCharFromActiveField() {
        if (activeInputField == 0) {
            if (currentSSID.length() > 0) {
                currentSSID = currentSSID.substring(0, currentSSID.length() - 1);
            }
        } else if (activeInputField == 1) {
            if (currentPassword.length() > 0) {
                currentPassword = currentPassword.substring(0, currentPassword.length() - 1);
            }
        }
        drawCurrentState();
    }
    
    void drawCurrentWeather(String location, float temp, int humidity, float windSpeed,
                           String description, float feelsLike, int weatherId) {
        graphics.drawCurrentWeather(location, temp, humidity, windSpeed, description, feelsLike, weatherId);
    }
    
    void drawHourlyForecast(vector<HourlyData>& hours) {
        graphics.drawHourlyForecast(hours);
    }
    
    void drawForecast5Day(vector<ForecastDay>& days) {
        graphics.drawForecast5Day(days);
    }
    
    void drawForecast7DayWithPrecip(vector<ForecastDay>& days) {
        graphics.drawForecast7DayWithPrecip(days);
    }
    
    TouchHandler& getTouch() {
        return touch;
    }
};

#endif
