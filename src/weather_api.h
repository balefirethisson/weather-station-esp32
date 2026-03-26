#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

using namespace std;

struct WeatherData {
    float temp;
    float feelsLike;
    int humidity;
    float windSpeed;
    String description;
    int weatherId;
    float precipitation;
};

struct HourlyData {
    String time;
    float temp;
    int weatherId;
    String description;
    float precipProb;
    float windSpeed;
};

struct ForecastDay {
    String date;
    float tempMax;
    float tempMin;
    String description;
    int weatherId;
    float precipitation;
    float precipChance;
};

class WeatherAPI {
private:
    String apiKey;
    String latitude;
    String longitude;
    WeatherData currentWeather;
    vector<HourlyData> hourlyForecast;
    vector<ForecastDay> forecast5day;
    vector<ForecastDay> forecast7day;
    
public:
    WeatherAPI(String key) : apiKey(key) {}
    
    void setLocation(String lat, String lon) {
        latitude = lat;
        longitude = lon;
    }
    
    bool fetchCurrentWeather() {
        if (!WiFi.isConnected()) return false;
        
        String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + latitude + 
                     "&lon=" + longitude + "&units=metric&appid=" + apiKey;
        
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode != 200) {
            http.end();
            return false;
        }
        
        String payload = http.getString();
        http.end();
        
        DynamicJsonDocument doc(2048);
        if (deserializeJson(doc, payload)) {
            return false;
        }
        
        currentWeather.temp = celsiusToFahrenheit(doc["main"]["temp"].as<float>());
        currentWeather.feelsLike = celsiusToFahrenheit(doc["main"]["feels_like"].as<float>());
        currentWeather.humidity = doc["main"]["humidity"].as<int>();
        currentWeather.windSpeed = doc["wind"]["speed"].as<float>() * 2.237;
        currentWeather.description = doc["weather"][0]["main"].as<String>();
        currentWeather.weatherId = doc["weather"][0]["id"].as<int>();
        currentWeather.precipitation = doc["rain"]["1h"].isNull() ? 0 : doc["rain"]["1h"].as<float>();
        
        return true;
    }
    
    bool fetchHourlyForecast() {
        if (!WiFi.isConnected()) return false;
        
        String url = "https://api.openweathermap.org/data/2.5/forecast?lat=" + latitude + 
                     "&lon=" + longitude + "&units=metric&appid=" + apiKey;
        
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode != 200) {
            http.end();
            return false;
        }
        
        String payload = http.getString();
        http.end();
        
        DynamicJsonDocument doc(8192);
        if (deserializeJson(doc, payload)) {
            return false;
        }
        
        hourlyForecast.clear();
        JsonArray list = doc["list"].as<JsonArray>();
        
        for (int i = 0; i < list.size() && i < 9; i++) {
            JsonObject item = list[i].as<JsonObject>();
            
            HourlyData hourly;
            hourly.time = item["dt_txt"].as<String>();
            hourly.time = hourly.time.substring(11, 16);
            hourly.temp = celsiusToFahrenheit(item["main"]["temp"].as<float>());
            hourly.weatherId = item["weather"][0]["id"].as<int>();
            hourly.description = item["weather"][0]["main"].as<String>();
            hourly.precipProb = item["pop"].as<float>() * 100;
            hourly.windSpeed = item["wind"]["speed"].as<float>() * 2.237;
            
            hourlyForecast.push_back(hourly);
        }
        
        return true;
    }
    
    bool fetchForecast() {
        if (!WiFi.isConnected()) return false;
        
        String url = "https://api.openweathermap.org/data/2.5/forecast?lat=" + latitude + 
                     "&lon=" + longitude + "&units=metric&appid=" + apiKey;
        
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode != 200) {
            http.end();
            return false;
        }
        
        String payload = http.getString();
        http.end();
        
        DynamicJsonDocument doc(8192);
        if (deserializeJson(doc, payload)) {
            return false;
        }
        
        forecast5day.clear();
        JsonArray list = doc["list"].as<JsonArray>();
        set<String> processedDates;
        
        for (JsonObject item : list) {
            if (processedDates.size() >= 5) break;
            
            String dateStr = item["dt_txt"].as<String>();
            String dateOnly = dateStr.substring(0, 10);
            
            if (processedDates.count(dateOnly)) continue;
            processedDates.insert(dateOnly);
            
            ForecastDay day;
            day.date = dateOnly;
            day.tempMax = celsiusToFahrenheit(item["main"]["temp_max"].as<float>());
            day.tempMin = celsiusToFahrenheit(item["main"]["temp_min"].as<float>());
            day.description = item["weather"][0]["main"].as<String>();
            day.weatherId = item["weather"][0]["id"].as<int>();
            day.precipitation = item["rain"]["3h"].isNull() ? 0 : item["rain"]["3h"].as<float>();
            day.precipChance = item["pop"].as<float>() * 100;
            
            forecast5day.push_back(day);
        }
        
        return true;
    }
    
    WeatherData getCurrentWeather() { return currentWeather; }
    vector<HourlyData> getHourlyForecast() { return hourlyForecast; }
    vector<ForecastDay> getForecast5Day() { return forecast5day; }
    
private:
    float celsiusToFahrenheit(float celsius) {
        return (celsius * 9.0 / 5.0) + 32.0;
    }
};

#endif
