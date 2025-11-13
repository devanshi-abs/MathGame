// ConfigManager.cpp
#include "ConfigManager.h"
#include "json/error/en.h"
#include <sstream>

USING_NS_CC;
using namespace rapidjson;

ConfigManager* ConfigManager::_instance = nullptr;

ConfigManager* ConfigManager::getInstance() {
    if (!_instance) {
        _instance = new ConfigManager();
    }
    return _instance;
}

bool ConfigManager::loadConfig(const std::string& fileName) {
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string jsonData = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    _doc.Parse(jsonData.c_str());
    if (_doc.HasParseError()) {
        CCLOG("Config JSON parse error: %s (offset %zu)", GetParseError_En(_doc.GetParseError()), _doc.GetErrorOffset());
        return false;
    }
    
    CCLOG("ConfigManager: Successfully loaded %s", fileName.c_str());
    return true;
}

rapidjson::Value* ConfigManager::getValueForPath(const std::string& path) {
    std::stringstream ss(path);
    std::string segment;
    rapidjson::Value* value = &_doc;
    
    while (std::getline(ss, segment, '.')) {
        if (!value->IsObject() || !value->HasMember(segment.c_str()))
            return nullptr;
        value = &(*value)[segment.c_str()];
    }
    return value;
}

bool ConfigManager::hasKey(const std::string& key) {
    return getValueForPath(key) != nullptr;
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) {
    auto v = getValueForPath(key);
    return (v && v->IsString()) ? v->GetString() : defaultValue;
}

float ConfigManager::getFloat(const std::string& key, float defaultValue) {
    auto v = getValueForPath(key);
    return (v && v->IsNumber()) ? v->GetFloat() : defaultValue;
}

int ConfigManager::getInt(const std::string& key, int defaultValue) {
    auto v = getValueForPath(key);
    return (v && v->IsInt()) ? v->GetInt() : defaultValue;
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) {
    auto v = getValueForPath(key);
    return (v && v->IsBool()) ? v->GetBool() : defaultValue;
}

Vec2 ConfigManager::getVec2(const std::string& key, Vec2 defaultValue) {
    auto v = getValueForPath(key);
    if (v && v->IsObject() && v->HasMember("x") && v->HasMember("y")) {
        return Vec2((*v)["x"].GetFloat(), (*v)["y"].GetFloat());
    }
    return defaultValue;
}
const rapidjson::Value& ConfigManager::getArray(const std::string& key) {
    static rapidjson::Value emptyArray(rapidjson::kArrayType);  // Safe fallback
    auto v = getValueForPath(key);
    if (v && v->IsArray()) {
        return *v;
    }
    return emptyArray;
}
