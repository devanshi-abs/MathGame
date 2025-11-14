// ConfigManager.h
#pragma once
#include "cocos2d.h"
#include "json/document.h"

class ConfigManager {
public:
    static ConfigManager* getInstance();
    
    void loadConfig(const std::string& fileName);
    bool hasKey(const std::string& key);
    
    // --- Typed Getters ---
    std::string getString(const std::string& key, const std::string& defaultValue = "");
    float getFloat(const std::string& key, float defaultValue = 0.0f);
    int getInt(const std::string& key, int defaultValue = 0);
    bool getBool(const std::string& key, bool defaultValue = false);
    cocos2d::Vec2 getVec2(const std::string& key, cocos2d::Vec2 defaultValue = cocos2d::Vec2::ZERO);
    const rapidjson::Value& getArray(const std::string& key);

    ConfigManager() = default;
    static ConfigManager* _instance;
    rapidjson::Document _doc;
    
    // Helper to access nested JSON paths like "character.eyeBlink.blinkDelay"
    rapidjson::Value* getValueForPath(const std::string& path);
};
