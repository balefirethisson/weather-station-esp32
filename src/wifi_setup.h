#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include <EEPROM.h>

class WiFiSetup {
private:
    String ssid = "";
    String password = "";
    bool setupComplete = false;
    const int EEPROM_SIZE = 512;
    
public:
    void begin() {
        EEPROM.begin(EEPROM_SIZE);
        loadFromEEPROM();
        
        if (!ssid.isEmpty() && !password.isEmpty()) {
            connectToWiFi();
        }
    }
    
    void connectToWiFi() {
        if (ssid.isEmpty()) return;
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            attempts++;
        }
        
        setupComplete = (WiFi.status() == WL_CONNECTED);
    }
    
    void setSsid(const String& s) {
        ssid = s;
        saveToEEPROM();
    }
    
    void setPassword(const String& p) {
        password = p;
        saveToEEPROM();
    }
    
    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
    
    String getIP() {
        return WiFi.localIP().toString();
    }
    
    void forget() {
        EEPROM.clear();
        EEPROM.commit();
        ssid = "";
        password = "";
    }
    
private:
    void saveToEEPROM() {
        EEPROM.clear();
        int addr = 0;
        
        EEPROM.write(addr, ssid.length());
        addr++;
        
        for (int i = 0; i < ssid.length(); i++) {
            EEPROM.write(addr + i, ssid[i]);
        }
        addr += 32;
        
        EEPROM.write(addr, password.length());
        addr++;
        
        for (int i = 0; i < password.length(); i++) {
            EEPROM.write(addr + i, password[i]);
        }
        
        EEPROM.commit();
    }
    
    void loadFromEEPROM() {
        int addr = 0;
        int ssidLen = EEPROM.read(addr);
        addr++;
        
        if (ssidLen > 0 && ssidLen < 32) {
            ssid = "";
            for (int i = 0; i < ssidLen; i++) {
                ssid += (char)EEPROM.read(addr + i);
            }
        }
        addr += 32;
        
        int passLen = EEPROM.read(addr);
        addr++;
        
        if (passLen > 0 && passLen < 64) {
            password = "";
            for (int i = 0; i < passLen; i++) {
                password += (char)EEPROM.read(addr + i);
            }
        }
    }
};

#endif
