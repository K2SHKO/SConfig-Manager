#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <shared_mutex>

class ConfigManager {
public:
    static ConfigManager& instance();

    void setPath(const std::string& path);
    void load(const std::string& path);
    void save() const;

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key, const std::string& def = "") const;

    void registerDefault(const std::string& key, const std::string& def);
    void subscribe(const std::string& key, std::function<void(const std::string&)> cb);

private:
    ConfigManager() = default;

    mutable std::shared_mutex mutex_;
    std::string path_ = "config.ini";
    std::unordered_map<std::string, std::string> data_;
    std::unordered_map<std::string, std::string> defaults_;
    std::unordered_map<std::string, std::function<void(const std::string&)>> subscribers_;

public:
    //std::string config = "config.ini";
};