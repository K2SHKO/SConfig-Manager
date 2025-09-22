#pragma once
#include <functional>
struct Initializer {
	Initializer(std::function<void()> f) { f(); }
};