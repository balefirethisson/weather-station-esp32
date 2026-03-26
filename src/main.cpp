#include <Arduino.h>
#include "config.h"
#include "wifi_setup.h"
#include "weather_api.h"
#include "ui_manager.h"

WiFiSetup wifiSetup;
UIManager uiManager;
WeatherAPI weatherAPI(OPENWEATHER_API_KEY);

UIState currentUIState = WIFI_SETUP;
unsigned long lastWeatherUpdate = 0;
unsigned long lastDisplayUpdate = 0;
int touchStartX = 0;
int touchStartY = 0;
unsigned long touchStartTime = 0;

WeatherData currentWeather;
vector<HourlyData> hourlyForecast;
vector<ForecastDay> forecast;

const int DISPLAY_UPDATE_INTERVAL = 100;

void handleWiFiSetup() {
    // Check if web portal is active
    if (wifiSetup.isWebPortalActive()) {
        uiManager.drawWiFiSetupScreen("", "", 0, true);
        delay(1000);
        return;
    }
    
    // Manual display-based setup
    TouchHandler& touch = uiManager.getTouch();
    
    if (touch.isTouched()) {
        TS_Point p = touch.getTouchPoint();
        
        // Check if tapping on SSID field
        if (p.y > 90 && p.y < 120) {
            uiManager.setActiveField(0);
        } 
        // Check if tapping on Password field
        else if (p.y > 160 && p.y < 190) {
            uiManager.setActiveField(1);
        }
        
        if (touchStartTime == 0) {
            touchStartX = p.x;
            touchStartY = p.y;
            touchStartTime = millis();
        } else if (millis() - touchStartTime > 100) {
            int deltaX = p.x - touchStartX;
            
            // Swipe right to connect
            if (deltaX > 80 && abs(p.y - touchStartY) < 40 && !uiManager.getSSID().isEmpty()) {
                wifiSetup.manualConnect(uiManager.getSSID(), uiManager.getPassword());
                
                uiManager.setState(LOADING);
                currentUIState = LOADING;
                touchStartTime = 0;
                
                // Wait for connection
                for (int i = 0; i < 30; i++) {
                    uiManager.drawCurrentState();
                    delay(200);
                    if (wifiSetup.isConnected()) {
                        break;
                    }
                }
                
                if (wifiSetup.isConnected()) {
                    Serial.println("WiFi connected!");
                    Serial.println("IP: " + wifiSetup.getIP());
                    
                    weatherAPI.setLocation(DEFAULT_LAT, DEFAULT_LON);
                    
                    delay(1000);
                    if (weatherAPI.fetchCurrentWeather()) {
                        weatherAPI.fetchHourlyForecast();
                        weatherAPI.fetchForecast();
                        
                        currentWeather = weatherAPI.getCurrentWeather();
                        hourlyForecast = weatherAPI.getHourlyForecast();
                        forecast = weatherAPI.getForecast5Day();
                        
                        uiManager.setState(CURRENT_WEATHER);
                        currentUIState = CURRENT_WEATHER;
                        lastWeatherUpdate = millis();
                    }
                } else {
                    Serial.println("WiFi connection failed");
                    uiManager.setState(WIFI_SETUP);
                    currentUIState = WIFI_SETUP;
                }
            }
        }
    } else {
        touchStartTime = 0;
    }
}

void handleWeatherNavigation() {
    TouchHandler& touch = uiManager.getTouch();
    
    if (touch.isTouched()) {
        TS_Point p = touch.getTouchPoint();
        
        if (touchStartTime == 0) {
            touchStartX = p.x;
            touchStartY = p.y;
            touchStartTime = millis();
        } else if (millis() - touchStartTime > 100) {
            int deltaX = p.x - touchStartX;
            
            if (abs(deltaX) > 80 && abs(p.y - touchStartY) < 60) {
                if (deltaX > 0) {
                    // Swipe right - go to previous screen
                    switch (currentUIState) {
                        case HOURLY_FORECAST:
                            uiManager.setState(CURRENT_WEATHER);
                            currentUIState = CURRENT_WEATHER;
                            break;
                        case FORECAST_5DAY:
                            uiManager.setState(HOURLY_FORECAST);
                            currentUIState = HOURLY_FORECAST;
                            break;
                        case FORECAST_7DAY:
                            uiManager.setState(FORECAST_5DAY);
                            currentUIState = FORECAST_5DAY;
                            break;
                        default:
                            break;
                    }
                } else {
                    // Swipe left - go to next screen
                    switch (currentUIState) {
                        case CURRENT_WEATHER:
                            uiManager.setState(HOURLY_FORECAST);
                            currentUIState = HOURLY_FORECAST;
                            break;
                        case HOURLY_FORECAST:
                            uiManager.setState(FORECAST_5DAY);
                            currentUIState = FORECAST_5DAY;
                            break;
                        case FORECAST_5DAY:
                            uiManager.setState(FORECAST_7DAY);
                            currentUIState = FORECAST_7DAY;
                            break;
                        default:
                            break;
                    }
                }
                touchStartTime = 0;
                lastDisplayUpdate = 0;
            }
        }
    } else {
        touchStartTime = 0;
    }
}

void updateDisplay() {
    unsigned long now = millis();
    
    if (now - lastDisplayUpdate < DISPLAY_UPDATE_INTERVAL) {
        return;
    }
    
    lastDisplayUpdate = now;
    
    switch (currentUIState) {
        case CURRENT_WEATHER:
            uiManager.drawCurrentWeather(
                DEFAULT_CITY,
                currentWeather.temp,
                currentWeather.humidity,
                currentWeather.windSpeed,
                currentWeather.description,
                currentWeather.feelsLike,
                currentWeather.weatherId
            );
            break;
            
        case HOURLY_FORECAST:
            uiManager.drawHourlyForecast(hourlyForecast);
            break;
            
        case FORECAST_5DAY:
            uiManager.drawForecast5Day(forecast);
            break;
            
        case FORECAST_7DAY:
            uiManager.drawForecast7DayWithPrecip(forecast);
            break;
            
        case WIFI_SETUP:
            uiManager.drawCurrentState();
            break;
            
        case LOADING:
            uiManager.drawCurrentState();
            break;
            
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\nWeather Station Starting...");
    Serial.println("Initializing WiFi...");
    
    uiManager.begin();
    wifiSetup.begin();
    
    if (wifiSetup.isConnected()) {
        Serial.println("WiFi already connected!");
        weatherAPI.setLocation(DEFAULT_LAT, DEFAULT_LON);
        delay(1000);
        
        if (weatherAPI.fetchCurrentWeather()) {
            weatherAPI.fetchHourlyForecast();
            weatherAPI.fetchForecast();
            
            currentWeather = weatherAPI.getCurrentWeather();
            hourlyForecast = weatherAPI.getHourlyForecast();
            forecast = weatherAPI.getForecast5Day();
            
            uiManager.setState(CURRENT_WEATHER);
            currentUIState = CURRENT_WEATHER;
            lastWeatherUpdate = millis();
        }
    } else {
        Serial.println("Not connected - showing setup screen");
        uiManager.setState(WIFI_SETUP);
        currentUIState = WIFI_SETUP;
    }
}

void loop() {
    switch (currentUIState) {
        case WIFI_SETUP:
            handleWiFiSetup();
            updateDisplay();
            delay(50);
            break;
            
        case LOADING:
            updateDisplay();
            delay(200);
            break;
            
        case CURRENT_WEATHER:
        case HOURLY_FORECAST:
        case FORECAST_5DAY:
        case FORECAST_7DAY:
            handleWeatherNavigation();
            updateDisplay();
            
            if (millis() - lastWeatherUpdate > WEATHER_UPDATE_INTERVAL) {
                if (wifiSetup.isConnected()) {
                    weatherAPI.fetchCurrentWeather();
                    weatherAPI.fetchHourlyForecast();
                    weatherAPI.fetchForecast();
                    
                    currentWeather = weatherAPI.getCurrentWeather();
                    hourlyForecast = weatherAPI.getHourlyForecast();
                    forecast = weatherAPI.getForecast5Day();
                    
                    lastWeatherUpdate = millis();
                    lastDisplayUpdate = 0;
                }
            }
            
            delay(50);
            break;
    }
}