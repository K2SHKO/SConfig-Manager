#pragma once
#include "../include/Initializer.h"
#include "../include/ConfigManager.h"
#define REGISTER_SETTING(Name, Key, DefaultValue) namespace {  static Initializer initializer_##Name([]() {ConfigManager::instance().registerDefault(Key, DefaultValue); \}); \}