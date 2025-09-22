#include "../include/ConfigManager.h"
#include "../include/RegisterSetting.h"
#include "../include/Initializer.h"
#include <iostream>
#include <thread>
#include <Windows.h>

            /*                    NOT USING IT ANYMORE;
           
REGISTER_SETTING(f1_toggle, "feature.f1_enabled", "0")
REGISTER_SETTING(f2_mode, "feature.f2_mode", "0")
REGISTER_SETTING(f3_level, "feature.f3_level", "1")
            
            */

int main() {
    ConfigManager::instance().registerDefault("feature.f1_enabled", "0");
    ConfigManager::instance().registerDefault("feature.f2_mode", "0");
    ConfigManager::instance().registerDefault("feature.f3_level", "1");

    ConfigManager::instance().setPath("config.ini");
    ConfigManager::instance().load("config.ini");

    ConfigManager::instance().subscribe("feature.f1_enabled", [](const std::string& v) {
#ifdef _DEBUG
        printf_s("[#F1] Value has changed: %s\n", (std::string*)v.c_str());
#endif
        });
    ConfigManager::instance().subscribe("feature.f2_mode", [](const std::string& v) {
#ifdef _DEBUG
        printf_s("[#F2] Value has changed: %s\n", (std::string*)v.c_str());
#endif
        });
    ConfigManager::instance().subscribe("feature.f3_level", [](const std::string& v) {
#ifdef _DEBUG
        printf_s("[#F3] Value has changed: %s\n", (std::string*)v.c_str());
#endif
        });

    printf("Press F1/F2/F3 to change settings, ESC/Q to exit.\n");

    bool running = true;
    while (running) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x1) {
#ifdef _DEBUG
            printf("[DEBUG] ESC pressed\n");
#endif
            running = false;
        }
        if (GetAsyncKeyState('Q') & 0x1) {
#ifdef _DEBUG
            printf("[DEBUG] Q pressed\n");
#endif
            running = false;
        }
        if (GetAsyncKeyState(VK_F1) & 0x1) {
            auto cur = ConfigManager::instance().get("feature.f1_enabled", "0");
            auto newVal = (cur == "1") ? "0" : "1";
            ConfigManager::instance().set("feature.f1_enabled", newVal);
        }
        if (GetAsyncKeyState(VK_F2) & 0x1) {
            auto cur = ConfigManager::instance().get("feature.f2_mode", "0");
            int m = std::stoi(cur);
            m = (m + 1) % 3;
            auto newVal = std::to_string(m);
            ConfigManager::instance().set("feature.f2_mode", newVal);
        }
        if (GetAsyncKeyState(VK_F3) & 0x1) {
            auto cur = ConfigManager::instance().get("feature.f3_level", "1");
            int l = std::stoi(cur);
            l = (std::min)(5, l + 1);
            auto newVal = std::to_string(l);
            ConfigManager::instance().set("feature.f3_level", newVal);
        }
        //Sleep(50);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}