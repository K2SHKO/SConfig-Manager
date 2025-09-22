#include "../include/ConfigManager.h"
#include <fstream>
#include <sstream>

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

void ConfigManager::setPath(const std::string& path) {
    if (path.empty()) // if the config file is not in arguments
        path == path_;

    path_ = path;
}

void ConfigManager::load(const std::string& path) {

    if (path.empty()) // if the config file is not in arguments
        path == path_;
    
    path_ = path;
    
    data_.clear();

    std::ifstream ifs(path_);
    if (!ifs.is_open()) {
        for (auto const& kv : defaults_) data_[kv.first] = kv.second;
        save();
        return;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        auto key = line.substr(0, pos);
        auto val = line.substr(pos + 1);
        data_[key] = val;
    }

    for (auto const& kv : defaults_) {
        if (data_.find(kv.first) == data_.end()) data_[kv.first] = kv.second;
    }

    save();
}

void ConfigManager::save() const {
    std::ofstream ofs(path_, std::ios::trunc);
    for (auto const& kv : data_) ofs << kv.first << "=" << kv.second << "\n";
}

void ConfigManager::set(const std::string& key, const std::string& value) {
    data_[key] = value;
    save();

    auto it = subscribers_.find(key);
    if (it != subscribers_.end() && it->second) it->second(value);
}

std::string ConfigManager::get(const std::string& key, const std::string& def) const {
    auto it = data_.find(key);
    if (it != data_.end()) return it->second;
    auto it2 = defaults_.find(key);
    if (it2 != defaults_.end()) return it2->second;
    return def;
}

void ConfigManager::registerDefault(const std::string& key, const std::string& def) {
    defaults_[key] = def;
    if (data_.find(key) == data_.end()) data_[key] = def;
}

void ConfigManager::subscribe(const std::string& key, std::function<void(const std::string&)> cb) {
    subscribers_[key] = std::move(cb);
}
